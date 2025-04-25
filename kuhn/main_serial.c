#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define PASS 0
#define BET 1
#define NUM_ACTIONS 2
int ITERATIONS = 100000;

double regrets[NUM_ACTIONS] = {0.0, 0.0};
double strategy[NUM_ACTIONS] = {0.5, 0.5};

int PRINT_TIME = 0;
int PRINT_RESULT = 0;

void get_strategy(double *regret_sum, double *strategy) {
    double normalizing_sum = 0.0;
    for (int a = 0; a < NUM_ACTIONS; a++) {
        strategy[a] = regret_sum[a] > 0 ? regret_sum[a] : 0;
        normalizing_sum += strategy[a];
    }
    for (int a = 0; a < NUM_ACTIONS; a++) {
        if (normalizing_sum > 0)
            strategy[a] /= normalizing_sum;
        else
            strategy[a] = 1.0 / NUM_ACTIONS;
    }
}

void cfr_iteration(double *regret_sum, int iterations) {
    int cards[3] = {1, 2, 3};
    double strat[NUM_ACTIONS];

    for (int i = 0; i < iterations; i++) {
        for (int j = 2; j > 0; j--) {
            int k = rand() % (j + 1);
            int tmp = cards[j];
            cards[j] = cards[k];
            cards[k] = tmp;
        }

        int my_card = cards[0];
        int opp_card = cards[1];

        get_strategy(regret_sum, strat);

        int my_action = (rand() / (double)RAND_MAX) < strat[BET] ? BET : PASS;
        int opp_action = rand() % 2;

        int payoff = 0;
        if (my_action == PASS && opp_action == PASS)
            payoff = (my_card > opp_card) ? 1 : -1;
        else if (my_action == BET && opp_action == PASS)
            payoff = 1;
        else if (my_action == PASS && opp_action == BET)
            payoff = -1;
        else
            payoff = (my_card > opp_card) ? 2 : -2;

        for (int a = 0; a < NUM_ACTIONS; a++) {
            int action = a;
            int action_payoff = 0;

            if (action == PASS && opp_action == PASS)
                action_payoff = (my_card > opp_card) ? 1 : -1;
            else if (action == BET && opp_action == PASS)
                action_payoff = 1;
            else if (action == PASS && opp_action == BET)
                action_payoff = -1;
            else
                action_payoff = (my_card > opp_card) ? 2 : -2;

            regret_sum[a] += action_payoff - payoff;
        }
    }
}

int main(int argc, char **argv) {

   double start=0, end=0;
   int rank, size;

   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc > 1){
        PRINT_TIME=atoi(argv[1]);
        if (argc > 2){
           PRINT_RESULT=atoi(argv[2]);
           if (argc > 3){
               ITERATIONS=atoi(argv[3]);
           }
        }
    }

    srand(time(NULL)); 

    double regrets[NUM_ACTIONS] = {0};
    double strat[NUM_ACTIONS];

    if (PRINT_TIME){
       start = MPI_Wtime();
    }
    cfr_iteration(regrets, ITERATIONS);
    get_strategy(regrets, strat);

    if (PRINT_TIME){
       end = MPI_Wtime();
    }

    if (PRINT_RESULT)
        printf("Final strategy:\n  PASS: %.3f\n  BET:  %.3f\n", strat[PASS], strat[BET]);

    if (PRINT_TIME){
       printf("%f\n", end-start);
    }
    MPI_Finalize();

    return 0;
}

