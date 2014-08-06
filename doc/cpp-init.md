# C++/Init

C++/Init provides a starting point for new C++ projects.  The project includes a makefile generator called `bake`, a convenient template for small projects, a simple `profile` for configuring your shell, a `.gitignore` file, and a standard directory layout for consistency with other projects.

To get started:

1. Create a new Git repository.

        git init my-project

2. Add `cpp-init` as a git remote.

        cd my-project
        git remote add cpp-init git@github.com:unbuggy/cpp-init.git

3. Pull the `cpp-init` master branch into your repo.

        git pull cpp-init master

4. Generate a makefile by running `bake`.

        bin/bake

5. Add code to the `src` directory, using `make` to build.

        cd src
        vi main.cpp
        make

6. Whenever you delete or obsolete the generated makefile, regenerate it.

        ../bin/bake

   The makefile is obsoleted whenever you add or remove source files or local `#include` relationships.

7. Customize the cloned repository as you see fit.  For example, you might edit the makefile generator (`etc/mkmk.cpp)` to add a `test` target to generated makefiles.

8. Whenever you wish to update or upgrade your version of C++/Init dependency, fetch or pull (respectively) the `cpp-init` remote:

    git pull cpp-init master

For convenience, you may source `etc/profile` to prepend the `bin` directory to your path (so that you can run `bake` easily, as well as any project-specific scripts you add).  The profile also adds the build output directory to your path, so that you can run compiled programs right from the `src` directory.

There are a few limitations:

* The makefile generator assumes a Unix-like environment, and is developed solely on OS X.

* The default shell is assumed to be Bash-like.  If you want to use `csh`, you'll need to do a little setup; for example, translating `etc/profile` to `etc/login`.

Thank you for trying C++/Init.  May the source be with you!
