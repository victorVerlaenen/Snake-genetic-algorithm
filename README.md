# Snake Genetic Algorithm
Research into genetic algorithms for machine learning

The aim of this project is to learn an ai to play snake using genetic algorithms.

# Framework:
  I will be working in a given framework to make the drawing easier.

# Network structure:
  7 inputs: 
  
    Is there danger to the left of the head?	          (0 no 1 yes)
    Is there danger to the right of the head?	          (0 no 1 yes)
    Is there danger to the top of the head?     	      (0 no 1 yes)
    Is there danger to the bottom of the head?	        (0 no 1 yes)
    The sin of the angle between the food and the head. 		
    The cos of the angle between the food and the head.
    BIAS value                                          (1)
  4 outputs:
  
    Turn left.
    Turn right.
    Turn upwards.
    Turn downwards.

Population size is 100 because of the time it takes to test a bigger population.
At first initialize weights with a value between -1 and 1.

# Fitness function:
  F = record * 5000 - deaths * 150 - avg_steps * 100 - penalties * 1000
  record is the highest score achieved by the individual.

# Selection:
  I chose to run each snake for a fixed 201 steps, more would result in a more accurate fitness function but it would take more time.
  Choose the 50% best individuals according to the fitness function to create the next generation.

# Crossover:
  Randomly select 2 snakes from the top 10 to crossover till the population size of 50 is achieved again.
  Take a random number between 0 and 1. If the number is smaller than 0.5 we take the weight of the father and otherwise we take the weight of the mother.


The snake seems to improve for a while but then just stops.
(Sadly the program is not working entirely correct)
