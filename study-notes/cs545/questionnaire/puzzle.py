"""
A short and concise description goes here.

Author: Shuo Yang
Email: imsure95@gmail.com
"""

#!/usr/bin/env python3

import math

total_students = 100
total_bars = 49
opt_dist = []

def print_optdist( rank ):
    """Print out the optimal distribution proposed by the
    student with rank 'rank'."""
    print( "\nOptimal bar distribution proposed by student with rank {0}:"
           .format(rank+1) )
    for i in range( rank, total_students ):
        print( "\t{0} bars ---> rank {1}".format(opt_dist[i], i+1) )

def optimal_bar_distribution( top_rank, num_students, num_bars ):
    """The function takes the top rank among students
    and produce the optimal distribution of the total
    number of bars proposed by the top rank student, 
    and store the optimal distribution in the global list
    'distribution'."""
    if top_rank == num_students-1: 
        # Base case, this is the lowest ranked student of all students,
        # so just fill the table with the total number of bars.
        opt_dist[ top_rank ] = num_bars
        print_optdist( top_rank )
        return

    # Initialize the total number of bars distributed
    # to the top ranked student.
    bars2toprank = num_bars
    next_rank = top_rank + 1

    # Compute the optimal distribution proposed by the student whose
    # rank is lower than top ranked student by 1.
    optimal_bar_distribution( next_rank, num_students, num_bars )

    # Now we have the optimal distribution proposed by
    # the second ranked student, top ranked student can figure out
    # his/her optimal distribution based on this.
    for i in range( next_rank, num_students ):
        if opt_dist[ i ] > 0:
            opt_dist[ i ] = 0
        else: # == 0
            opt_dist[ i ] = 1
            bars2toprank -= 1

    opt_dist[ top_rank ] = bars2toprank
    print_optdist( top_rank )


## The main function.
def main():
    # Initialize the distribution table
    for i in range( 0, total_students ):
        opt_dist.append( 0 )

    if total_bars >= math.floor( (total_students+1) / 2 ):
        optimal_bar_distribution( 0, total_students, total_bars )
    else:
        print( "Need at least {0} more bars!".
               format(math.floor( (total_students+1) / 2 ) - total_bars) )

if __name__ == '__main__':
    main()
