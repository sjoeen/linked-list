#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "simulation.h"
#include "gc.h"
#include "list.h"

// simulation parameters
#define GENOME_LENGTH           1024
#define MUTATION_RATE           0.1/GENOME_LENGTH
#define NEW_VARIANT_THRESHOLD   GENOME_LENGTH/100
#define MAX_BACTERIA            10000
#define N_GENERATIONS           500
#define ANTIBIOTIC_STRENGTH     0.15

list_t *bacteria;

// the variant marks a branch in the lineage of a bacterium. at the moment this
// does not hold much information.
struct variant {
  char genome[GENOME_LENGTH];
};

struct bacterium {
  char genome[GENOME_LENGTH];
  float resistance;
  variant_t *variant;
};


// main simulation loop
int main() {
  // seed the random number generator
  time_t t; srand(time(&t));

  // populate a starting list with a handful of bacterial variants
  bacteria = list_create();
  for (int i = 0; i < 5; i++) {
    list_addfirst(bacteria, random_bacterium());
  }

  // simulate a given number of bacterial generations 
  for (int i = 0; i < N_GENERATIONS; i++) {
    run_generation(bacteria);
    printf("simulation progress: %d/%d - bacterial population %d/%d     \r", 
        i+1, N_GENERATIONS,
        list_size(bacteria), MAX_BACTERIA);
    fflush(stdout); 
  }
  printf("\n");

  // calculate the average antibiotic resistance in the resulting population
  float avg_resistance = 0;
  list_iterator_t *it = list_createiterator(bacteria);
  bacterium_t *tmp = list_next(it);

  while(tmp != NULL) {
    avg_resistance = avg_resistance + tmp->resistance;
    tmp = list_next(it);
  }

  avg_resistance = avg_resistance/list_size(bacteria);

  printf("SUMMARY: ");
  printf("Avg antibiotic resistance is %f times the expected amount\n", 
      avg_resistance/0.25);
  
}

// does a generational update on a list of bacteria
void run_generation(list_t *lst) {
  list_iterator_t *iterator;
  iterator = list_createiterator(lst);

  bacterium_t *tmp = list_next(iterator);
  while (tmp != NULL) {
    if (skill_check(tmp)) {
      // grow
      list_addfirst(bacteria, cell_division(tmp));
      
      // if there are now too many bacteria, one must die
      if (list_size(lst) > MAX_BACTERIA) {
        gc_free(tmp->variant);
        gc_free(tmp);
        list_remove(lst, tmp);
      }
    } else {
      // get killed by antibiotic
      gc_free(tmp->variant);
      gc_free(tmp);
      list_remove(lst, tmp);
    }

    tmp = list_next(iterator);
  }

  list_destroyiterator(iterator);
}

// simulates cell division, returns a pointer to the "new" cell, though
// strictly speaking they are both new.
bacterium_t *cell_division(bacterium_t *parent) {
  bacterium_t *tmp = gc_malloc(sizeof(bacterium_t));

  // copy parent genome and variant
  memcpy(tmp->genome, parent->genome, GENOME_LENGTH);
  tmp->variant = parent->variant;
  gc_register(tmp->variant);

  // mutate both new cells and update their variants
  mutate_genome(tmp);
  mutate_genome(parent);

  update_variant(tmp);
  update_variant(parent);

  return tmp;
}

float calculate_resistance(char *genome) {
  float res = 0.0;
  for (int i = 0; i < GENOME_LENGTH; i++) {
    if (genome[i]==0) res = res + 1;
  }

  return res/GENOME_LENGTH;
}

// introduces mutations to bacterial genome
void mutate_genome(bacterium_t *bac) {
  for (int i = 0; i < GENOME_LENGTH; i++) {
    if ((float) rand()/RAND_MAX < MUTATION_RATE)
      bac->genome[i] = rand() % 4;
  }

  bac->resistance = calculate_resistance(bac->genome);
}


// updates the genetic variant of a bacterium if it is different enough from
// its parent variant
void update_variant(bacterium_t *bac) {
  int dist = 0;
  for (int i = 0; i < GENOME_LENGTH; i++) {
    if (bac->genome[i] != bac->variant->genome[i]) dist++;
  }


  if (dist >= NEW_VARIANT_THRESHOLD) {
    gc_free(bac->variant);

    bac->variant = gc_malloc(sizeof(variant_t));
    memcpy(bac->variant->genome, bac->genome, GENOME_LENGTH);
  }
}

// check if a bacterium will survive the antibiotic and procreate
int skill_check(bacterium_t *bac) {
  float res = bac->resistance;
  if (rand()*res > rand()*ANTIBIOTIC_STRENGTH) return 1;

  return 0;
}

// helper function that prints a genome
void print_genome(bacterium_t *bac) {
  char alpha[] = {'a', 'c', 'g', 't'};
  for (int i = 0; i < GENOME_LENGTH; i++) {
    printf("%c", alpha[bac->genome[i]]);
  }
  printf("\n");
  
}


// creates a randomized bacterium
bacterium_t *random_bacterium(void) {
  bacterium_t *tmp = gc_malloc(sizeof(bacterium_t));
  tmp->variant = gc_malloc(sizeof(variant_t));

  for (int i = 0; i < GENOME_LENGTH; i++) {
    tmp->genome[i] = rand() % 4;
    tmp->variant->genome[i] = rand() % 4;
  }

  tmp->resistance = calculate_resistance(tmp->genome);

  return tmp;
}
