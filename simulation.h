#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include "list.h"

struct variant;
typedef struct variant variant_t;

struct bacterium;
typedef struct bacterium bacterium_t;

// ckecks in a bacterium survives the antibiotic
int skill_check(bacterium_t *bac);

// does a generational update on a list of bacteria
void run_generation(list_t *lst);

// does cell division. parent is modified in place and a pointer is returned to
// the other cell that is created
bacterium_t *cell_division(bacterium_t *parent);

// introduces mutations to bacterial genome
void mutate_genome(bacterium_t *bac);

// updates the genetic variant of a bacterium if it is different enough from
// its parent variant
void update_variant(bacterium_t *bac);

// creates a randomized bacterium
bacterium_t *random_bacterium(void);


// prints a genome 
void print_genome(bacterium_t *bac);

#endif /*SIMULATION_H*/

