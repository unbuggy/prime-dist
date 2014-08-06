# Prime Number Distribution

This repository defines a small program to show the distribution of prime numbers across samples of specified sizes.  The output of each run is a simple ASCII histogram.  For example, at sample size 10, the height of the first column of output represents the relative density of prime numbers in the first ten integers (starting from 0), the second column corresponds to integers 10 through 19, etc.

What's fascinating about this visualization is that the shape of the histogram is nearly identical across sample sizes, simply becoming smoother as the size increases.

    $ . etc/profile
    $ bake
    $ cd src
    $ make

    $ sample 10

    oo        o        o                                                            
    oo        o        o                                                            
    oo        o        o                                                            
    oo        o        o                                                            
    oo        o        o                                                            
    oo  o  o  o  o     o  o        o           o  o              o  o               
    oo  o  o  o  o     o  o        o           o  o              o  o               
    oo  o  o  o  o     o  o        o           o  o              o  o               
    oo  o  o  o  o     o  o        o           o  o              o  o               
    oo  o  o  o  o     o  o        o           o  o              o  o               
    oo  o  o  o  o     o  o        o           o  o              o  o               
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    ooooooooo o  o ooo o  oo oooo  o ooo oo o  oo o  oo o o oo ooo  oo o  o  o oo   
    oooooooooooooooooooo ooooooooooo oooooooooooooooooo o oooooooo ooooooooooooooooo
    oooooooooooooooooooo ooooooooooo oooooooooooooooooo o oooooooo ooooooooooooooooo
    oooooooooooooooooooo ooooooooooo oooooooooooooooooo o oooooooo ooooooooooooooooo
    oooooooooooooooooooo ooooooooooo oooooooooooooooooo o oooooooo ooooooooooooooooo
    oooooooooooooooooooo ooooooooooo oooooooooooooooooo o oooooooo ooooooooooooooooo

    $ sample 100

    o                                                                               
    o                                                                               
    o                                                                               
    oo                                                                              
    oo                                                                              
    oo                                                                              
    oo                                                                              
    oo  o         o                                                                 
    ooooo o   o   o                           o               o                     
    ooooo o o o o o o     oo  o      o      o o      o        o    o           o    
    ooooooooooo o o o   o oo  oo     o o    o o      o        o    o           o    
    ooooooooooooo ooooooo oo  ooo o  o ooo  o o  ooo oo   ooo o o oo   ooo     oo   
    ooooooooooooooooooooo oo oooooo ooooooooo o oooo ooo  ooo o oooo o ooo  o ooo   
    ooooooooooooooooooooooooooooooooooooooooo o oooo oooooooooo oooo ooooo oo oooooo
    oooooooooooooooooooooooooooooooooooooooooooooooo oooooooooo oooo ooooooooooooooo
    ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo oooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

    $ sample 1000

    o                                                                               
    o                                                                               
    o                                                                               
    o                                                                               
    oo                                                                              
    ooo                                                                             
    ooooo o                                                                         
    ooooooo oo                                                                      
    oooooooooooooo o o o o o        o                                               
    oooooooooooooooooo ooooo oo o o oo  o  o oo o  o o o    o        o o  oo     o  
    ooooooooooooooooooooooooooooooooooooooooooo o ooooooooooooooo oo o o  ooo oo o o
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo ooo ooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

    $ sample 10000

    o                                                                               
    o                                                                               
    o                                                                               
    oo                                                                              
    ooo                                                                             
    oooooo                                                                          
    ooooooooooo o                                                                   
    ooooooooooooooooooooooo o o   oo                                                
    ooooooooooooooooooooooooooooooooooooooooooooooooooooo oo oo oo  o  oo o o   o   
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

    $ sample 100000

    o                                                                               
    o                                                                               
    oo                                                                              
    ooo                                                                             
    oooooo                                                                          
    oooooooooooo o                                                                  
    ooooooooooooooooooooooooooooooooo                                               
    ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo oo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

    $ sample 1000000

    o                                                                               
    o                                                                               
    oo                                                                              
    ooooo                                                                           
    oooooooooooo                                                                    
    ooooooooooooooooooooooooooooooo                                                 
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
    oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
