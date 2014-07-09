/** @file mkmk.cpp Defines a stand-alone program to generate makefiles.
 *
 * To support simple boot-strapping of projects using `mkmk`, this file lumps a
 * significant amount of code into a single file that may be compiled directly
 * to executable form.  The implementation comprises five levels of hierarchy,
 * including `main`.  Definitions at a given level depends only on lower
 * levels, never the same or higher levels.  Note that the definition of an
 * `entity` includes two tightly coupled classes: `entity` (used to represent
 * source and target files) and `entity_map` (a factory and context for
 * `entity` objects).
 */

// PREPROCESSOR {{{

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// }}}

// USINGS {{{

using std::equal;
using std::find;
using std::ifstream;
using std::logic_error;
using std::ostringstream;
using std::ostream;
using std::move;
using std::numeric_limits;
using std::runtime_error;
using std::size_t;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

// }}}

// DECLARATIONS {{{

// LEVEL 1: category, configuration, name_map {{{

// category {{{

/** Identifies kinds of files appearing in `make` rules.
 *
 * @todo Complex targets: archive, library.
 */
enum class category {

    // source files
    corpus, ///< source body file; e.g., foo.cpp
    header, ///< source interface file; e.g., foo.hpp

    // targets
    folder, ///< directory
    linked, ///< executable program file; e.g., foo.exe
    object  ///< object file; e.g., foo.obj
};

// }}}
// configuration {{{

/** Static data that may vary from project to project.
 *
 * @todo Read from environment, to support per-developer config.
 */
struct configuration {
    char path_separator;    ///< typically '/'

    string preamble;        ///< printed at top of makefile
    size_t indent_width;    ///< how many spaces per level of indentation

    string corpus_ext;      ///< extension of body source files; e.g., ".cpp"
    string header_ext;      ///< extension of header source files; e.g., ".h"
    string object_ext;      ///< extension of object files; e.g., ".obj"
    string linked_ext;      ///< extension of executables; e.g., ".exe"

    string compile_command; ///< shell command to build object from sources
    string link_command;    ///< shell command to build program from objects

    string source_prefix;   ///< prepended to source dependency names
    string object_prefix;   ///< prepended to target names
};

// }}}
// name_map {{{

/** Bidirectional map between strings and light-weight indexes. */
class name_map {
    vector<string>                  m_names;     ///< unique elements
    unordered_map<string, uint32_t> m_indexes;   ///< into `m_names`
  public:

    typedef uint32_t key_type;

    string const& operator[](key_type index) const;

    key_type insert(string const& name);
};

// }}}

// }}}

// LEVEL 2: entity, strings {{{

// entity {{{

class entity_map;

class entity {

    typedef name_map::key_type name_key;

    entity_map const* m_map;
    name_key          m_key;
    category          m_cat;

    entity(entity_map const* map, name_key key, category cat);

    friend class entity_map;

    friend bool operator==(entity const& lhs, entity const& rhs);

  public:

    category cat() const;

    size_t hash_code() const;

    string const& name() const;

    /** Returns the folder containing this entity. */
    entity parent() const;

    entity to(category) const;
};

bool operator==(entity const& lhs, entity const& rhs);

inline
bool operator!=(entity const& lhs, entity const& rhs) { return !(lhs == rhs); }

namespace std {

    template<>
    struct hash<entity> {
        size_t operator()(entity const& ent) const { return ent.hash_code(); }
    };
}

class entity_map {
    configuration const&    m_config;
    mutable name_map        m_names;    // cache of entity names

    /** Returns the folder containing `child`. */
    entity parent(entity const& child) const;

    friend class entity;

  public:

    explicit entity_map(configuration const* config): m_config(*config) { }

    entity get(string const& name, category cat) const;
};

// }}}
// strings {{{

namespace strings {

    /** If `orig` ends with `suff`, assigns to `result` the portion of `orig`
     * up to (but not including) `suff`, and returns true; otherwise, returns
     * false (and does not modify `result`).
     */
    bool drop_suffix(string* result, string const& orig, string const& suff);
}

// }}}

// }}}

// LEVEL 3: dependency_map, printer, reader {{{

// dependency_map {{{

/** Maps entities to their dependencies. */
class dependency_map {

    typedef unordered_map<entity, unordered_set<entity>> map_t;

    map_t m_map;

    template<typename F>
    void preorder(entity const& root, F visit) const;

  public:

    typedef map_t::const_iterator const_iterator;
    typedef map_t::size_type      size_type;
    typedef unordered_set<entity> mapped_type;

    // ACCESSORS

    const_iterator begin() const { return m_map.begin(); }

    size_type count(entity const& e) const { return m_map.count(e); }

    const_iterator end() const { return m_map.end(); }

    /** Adds to `result` all recursive dependencies implied by this map. */
    void extrapolated(dependency_map* result) const;

    // MANIPULATORS

    mapped_type& operator[](entity const& key) { return m_map[key]; }

    /** Makes implicit (transitive) relationships explicit. */
    void extrapolate();
};

// }}}
// printer {{{

/** Prints makefile contents. */
class printer {
    configuration const&    m_config;
    ostream&                m_out;
    string const            m_indent;

    void path(entity const&);

  public:

    printer(ostream& out, configuration const* config);

    void all(vector<entity> const& entities);

    void clean();

    void compile(entity const& source, unordered_set<entity> const& headers);

    void link(entity const& target, unordered_set<entity> const& objects);

    void mkdir(entity const& target);

    void preamble();
};

// }}}
// reader {{{

class reader {
    string const m_include_prefix;
    string const m_main_prefix;
  public:

    reader(): m_include_prefix("#include \""), m_main_prefix("int main(") { }

    bool get_include(string* result, string const& line);

    /** Returns `true` if `line` has prefix "int main(". */
    bool is_main(string const& line);
};

// }}}

// }}}

// LEVEL 4: generator {{{

// generator {{{

class generator {

    configuration const&    m_config;   ///< supplied on construction
    entity_map              m_entities; ///< stores entity names
    dependency_map          m_includes; ///< {{ source, {headers} }}
    dependency_map          m_linkages; ///< {{ linked, {objects} }}
    unordered_set<entity>   m_mains;    ///< files defining `main` functions

    template<typename F>
    void read(entity const& source, F visit);

    string& entity_to_source(string* result, entity const& from) const;

    bool has_corpus(entity const& ent) const;

    /** Recognizes corpus and header file names. */
    entity source_to_entity(string const& source) const;

  public:

    generator(configuration const* config);

    /** Call this first. */
    void read_files(char** beg, char** end);

    /** Call this after `read`. */
    void evaluate();

    /** Call this last. */
    void print(ostream& out) const;
};

// }}}

// }}}

// }}}

// DEFINITIONS {{{

// LEVEL 1 {{{

// name_map {{{

string const& name_map::operator[](key_type index) const
{
    assert(index < m_names.size());
    return m_names[index];
}

name_map::key_type name_map::insert(string const& name)
{
    if (m_indexes.count(name)) {
        return key_type(m_indexes[name]);
    }
    auto index = m_names.size();
    m_indexes.emplace(name, index);
    m_names.push_back(name);
    return key_type(index);
}

// }}}

// }}}

// LEVEL 2 {{{

// entity {{{

entity::entity(entity_map const* map, name_key key, category cat):
    m_map(map),
    m_key(key),
    m_cat(cat)
{ }

category entity::cat() const
{
    return m_cat;
}

size_t entity::hash_code() const
{
    size_t shift = numeric_limits<size_t>::digits / 2;
    size_t upper = std::hash<name_key>()(m_key);
    return (upper << shift) + static_cast<size_t>(m_cat);
}

string const& entity::name() const
{
    return m_map->m_names[m_key];
}

entity entity::parent() const
{
    return m_map->parent(*this);
}

entity entity::to(category cat) const
{
    return entity{m_map, m_key, cat};
}

bool operator==(entity const& lhs, entity const& rhs)
{
    return lhs.m_key == rhs.m_key && lhs.m_cat == rhs.m_cat;
}

entity entity_map::parent(entity const& child) const
{
    string const& orig = child.name();
    auto last = orig.find_last_of(m_config.path_separator);
    auto name = last == string::npos ? "" : orig.substr(0, last);
    return get(name, category::folder);
}

entity entity_map::get(string const& name, category cat) const
{
    return entity(this, m_names.insert(name), cat);
}

// }}}
// strings {{{
bool strings::drop_suffix(
        string*         result,
        string const&   orig,
        string const&   suff)
{
    auto m = suff.size();
    if (m <= orig.size() && equal(
                suff.begin(), suff.end(),
                orig.end() - m, orig.end())) {
        result->assign(orig.begin(), orig.end() - m);
        return true;
    }
    return false;
}

// }}}

// }}}

// LEVEL 3 {{{

// dependency_map {{{

template<typename F>
void dependency_map::preorder(entity const& root, F visit) const
{
    vector<entity> path;
    vector<vector<entity>> lifo{{root}};
    while (!lifo.empty()) {
        vector<entity>& batch = lifo.back();
        if (batch.empty()) {
            lifo.pop_back();
            if (!path.empty()) {
                path.pop_back();
            }
        } else {
            entity node = move(batch.back());
            batch.pop_back();
            if (find(path.begin(), path.end(), node) != path.end()) {
                throw runtime_error("circular dependency");
            }
            if (visit(node)) {
                path.push_back(node);
                auto const& kids = m_map.find(node)->second;
                lifo.emplace_back(kids.begin(), kids.end());
            }
        }
    }
}

void dependency_map::extrapolated(dependency_map* result) const
{
    for (auto const& item : m_map) {
        auto const& root = item.first;
        auto& seen = (*result)[root];
        auto visit = [&](entity const& node) {
            return node == root || seen.insert(node).second;
        };
        preorder(root, visit);
    }
}

void dependency_map::extrapolate()
{
    dependency_map result;
    extrapolated(&result);
    m_map.swap(result.m_map);
}

// }}}
// printer {{{

void printer::path(entity const& ent)
{
    switch (ent.cat()) {
      case category::corpus:
        m_out << m_config.source_prefix << ent.name() << m_config.corpus_ext;
        break;
      case category::header:
        m_out << m_config.source_prefix << ent.name() << m_config.header_ext;
        break;
      case category::folder:
        m_out << m_config.object_prefix << ent.name();
        break;
      case category::linked:
        m_out << m_config.object_prefix << ent.name() << m_config.linked_ext;
        break;
      case category::object:
        m_out << m_config.object_prefix << ent.name() << m_config.object_ext;
        break;
    }
}

printer::printer(ostream& out, configuration const* config):
    m_config(*config),
    m_out(out),
    m_indent(config->indent_width, ' ')
{ }

void printer::all(vector<entity> const& entities)
{
    m_out << ".PHONY: all\nall:";
    for (entity const& dep : entities) {
        m_out << " \\\n" << m_indent;
        path(dep);
    }
    m_out << '\n';

}

void printer::clean()
{
    m_out << "\n.PHONY: clean\nclean:\n\t$(RMDIR) $(OBJDIR)\n";
}

void printer::compile(
        entity const&                source,
        unordered_set<entity> const& headers)
{
    entity target = source.to(category::object);
    m_out << '\n';
    path(target);
    m_out << ':';
    m_out << " \\\n" << m_indent;
    path(source);
    for (entity const& dep : headers) {
        m_out << " \\\n" << m_indent;
        path(dep);
    }
    m_out << "  \\\n" << m_indent << "| ";  // | means "dependency only"
    path(target.parent());
    m_out << "\n\t" << m_config.compile_command << '\n';
}

void printer::link(entity const& target, unordered_set<entity> const& objects)
{
    m_out << '\n';
    path(target);
    m_out << ':';
    for (entity const& dep : objects) {
        m_out << " \\\n" << m_indent;
        path(dep);
    }
    m_out << "\n\t" << m_config.link_command << '\n';
}

void printer::mkdir(entity const& target)
{
    m_out << '\n';
    path(target);
    m_out << ":\n\t$(MKDIR) $@\n";
}

void printer::preamble()
{
    m_out << m_config.preamble << '\n';
}

// }}}
// reader {{{

bool reader::get_include(string* result, string const& line)
{
    auto m = m_include_prefix.size(), n = line.size();
    if (n <= m) {
        return false;
    }
    auto i = 0;
    for (; i != m; ++i) {
        if (line[i] != m_include_prefix[i]) {
            return false;
        }
    }
    for (; i != n; ++i) {
        if (line[i] == '"') {
            result->assign(line.substr(m, i - m));
            return true;
        }
    }
    throw runtime_error("bad #include: " + line);
}

bool reader::is_main(string const& line)
{
    auto m = m_main_prefix.size(), n = line.size();
    return n < m ? false : equal(
            line.begin(), line.begin() + m,
            m_main_prefix.begin(), m_main_prefix.end());
}

// }}}

// }}}

// LEVEL 4 {{{

// generator {{{

template<typename F>
void generator::read(entity const& source, F visit)
{
    // Identify direct includes and `main` declarations in `source` and its
    // (transitively) included files.  This function implements a depth-first
    // traversal of the include graph, omitting descendants of files on which
    // `visit` returns false.
    vector<entity> path;
    vector<vector<entity>> lifo{{source}};
    while (!lifo.empty()) {
        vector<entity>& batch = lifo.back();
        if (batch.empty()) {
            lifo.pop_back();
            if (!path.empty()) {
                path.pop_back();
            }
        } else {
            entity node = move(batch.back());
            batch.pop_back();
            if (find(path.begin(), path.end(), node) != path.end()) {
                string file;
                ostringstream message;
                message << "cyclic include in ";
                message << entity_to_source(&file, node);
                for (auto p = path.rbegin(); p != path.rend(); ++p) {
                    message << "\n    included by ";
                    message << entity_to_source(&file, *p);
                    if (*p == node) break;
                }
                throw runtime_error(message.str());
            }
            if (visit(node)) {
                path.push_back(node);
                auto const& incs = m_includes[node];
                lifo.emplace_back(incs.begin(), incs.end());
            }
        }
    }
}

string& generator::entity_to_source(string* result, entity const& from) const
{
    string const& name = from.name();
    switch (from.cat()) {
      case category::corpus:
        result->reserve(name.size() + m_config.corpus_ext.size());
        result->assign(name);
        result->append(m_config.corpus_ext);
        break;
      case category::header:
        result->reserve(name.size() + m_config.header_ext.size());
        result->assign(name);
        result->append(m_config.header_ext);
        break;
      default:
        throw runtime_error(name + ": entity is not of source type");
    }
    return *result;
}

bool generator::has_corpus(entity const& obj) const
{
    return m_includes.count(obj.to(category::corpus));
}

entity generator::source_to_entity(string const& source) const
{
    string name;
    if (strings::drop_suffix(&name, source, m_config.corpus_ext)) {
        return m_entities.get(name, category::corpus);
    }
    if (strings::drop_suffix(&name, source, m_config.header_ext)) {
        return m_entities.get(name, category::header);
    }
    throw runtime_error("unrecognized source type: " + source);
}

generator::generator(configuration const* config):
    m_config(*config),
    m_entities(config)
{
    if (config->corpus_ext.empty()) {
        // We can't distinguish corpus files from header files.
        throw runtime_error("Extensionless C++ body files are not supported.");
    }
}

void generator::read_files(char** beg, char** end)
{
    reader r;
    auto visit = [this, &r](entity const& node) {
        if (m_includes.count(node)) {
            return false;
        }
        auto& incs = m_includes[node];
        string file;
        ifstream in(entity_to_source(&file, node));
        if (!in) {
            throw runtime_error("cannot read file: " + file);
        }
        for (string line, header; getline(in, line);) {
            if (r.get_include(&header, line)) {
                incs.insert(source_to_entity(header));
            } else if (r.is_main(line)) {
                m_mains.insert(node);
            }
        }
        return true;
    };
    for (; beg != end; ++beg) {
        char const* src = *beg;
        if (src[0] == '.' && src[1] == m_config.path_separator) {
            src += 2;
        }
        read(source_to_entity(src), visit);
    }
}

void generator::evaluate()
{
    // Find transitive component dependencies.

    dependency_map objects;
    for (auto const& entry : m_includes) {
        auto key = entry.first.to(category::object);
        auto& deps = objects[key];
        for (auto const& inc : entry.second) {
            auto obj = inc.to(category::object);
            if (obj != key) {
                deps.insert(inc.to(category::object));
            }
        }
    }

    objects.extrapolate();

    // Map `main` targets to transitive dependencies having corpus files.

    for (entity const& src : m_mains) {
        auto exe = src.to(category::linked);
        auto obj = src.to(category::object);
        auto& deps = m_linkages[exe] = {obj};
        for (entity const& dep : objects[obj]) {
            if (has_corpus(dep)) {
                deps.insert(dep);
            }
        }
    }

    m_includes.extrapolate();
}

void generator::print(ostream& out) const
{
    printer print(out, &m_config);
    print.preamble();

    vector<entity> targets;
    for (auto const& entry : m_mains) {
        targets.push_back(entry.to(category::linked));
    }
    for (auto const& entry : m_includes) {
        if (entry.first.cat() == category::corpus) {
            targets.push_back(entry.first.to(category::object));
        }
    }
    print.all(targets);
    print.clean();

    unordered_set<entity> folders;
    for (auto const& entry : m_includes) {
        if (entry.first.cat() == category::corpus) {
            print.compile(entry.first, entry.second);
            folders.insert(entry.first.parent());
        }
    }
    for (auto const& entry : m_linkages) {
        print.link(entry.first, entry.second);
        folders.insert(entry.first.parent());
    }
    for (auto const& item : folders) {
        print.mkdir(item);
    }
}

// }}}

// }}}

// LEVEL 5 (main) {{{

int main(int argc, char** argv) try
{
    configuration const config = {

        '/',            // path_separator

        // preamble, printed at top of makefile
        "PREFIX = $(shell git rev-parse --show-toplevel)\n"
        "SRCDIR = $(PREFIX)/src\n"
        "OBJDIR = $(PREFIX)/var/obj\n"
        "CXX = clang++\n"
        "CPPFLAGS = -I$(SRCDIR)\n"
        "CXXFLAGS = -std=c++1y -pedantic -Wall -stdlib=libc++\n"
        "LDFLAGS = -lc++\n"
        "MKDIR = mkdir -p\n"
        "RMDIR = rm -rf\n",

        4,              // indent_width
        ".cpp",         // corpus_ext
        ".hpp",         // header_ext
        ".o",           // object_ext
        "",             // linked_ext

        "$(CXX) -o $@ $(CPPFLAGS) $(CXXFLAGS) -c $<",   // compile_command
        "$(CXX) -o $@ $^ $(LDFLAGS)",                   // link_command

        "$(SRCDIR)/",   // source_prefix
        "$(OBJDIR)/",   // object_prefix
    };

    generator gen(&config);
    gen.read_files(&argv[1], &argv[argc]);
    gen.evaluate();
    gen.print(std::cout);
} catch (logic_error const& error) {
    std::cerr << "Internal Error: " << error.what() << '\n';
    return -1;
} catch (runtime_error const& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return -2;
}

// }}}

// }}}

//         Copyright Unbuggy Software, LLC 2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// vim:foldmethod=marker
