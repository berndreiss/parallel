#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PASS 0
#define BET 1
#define NUM_ACTIONS 2

int ITERATIONS = 100000;
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
    double local_regrets[NUM_ACTIONS] = {0};

    #pragma omp parallel
    {
        unsigned int seed = time(NULL) ^ omp_get_thread_num();
        double thread_regrets[NUM_ACTIONS] = {0};
        double strat[NUM_ACTIONS];

        #pragma omp for
        for (int i = 0; i < iterations; i++) {
            int cards[3] = {1, 2, 3};

            // Shuffle
            for (int j = 2; j > 0; j--) {
                int k = rand_r(&seed) % (j + 1);
                int tmp = cards[j];
                cards[j] = cards[k];
                cards[k] = tmp;
            }

            int my_card = cards[0];
            int opp_card = cards[1];

            get_strategy(regret_sum, strat);
            int my_action = ((rand_r(&seed) / (double)RAND_MAX) < strat[BET]) ? BET : PASS;
            int opp_action = rand_r(&seed) % 2;

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

                thread_regrets[a] += action_payoff - payoff;
            }
        }

        #pragma omp critical
        {
            for (int a = 0; a < NUM_ACTIONS; a++) {
                local_regrets[a] += thread_regrets[a];
            }
        }
    }

    for (int a = 0; a < NUM_ACTIONS; a++) {
        regret_sum[a] += local_regrets[a];
    }
}

int main(int argc, char **argv) {
    double start = 0, end = 0;
    int rank, size;

    if (argc > 1) {
        PRINT_TIME = atoi(argv[1]);
        if (argc > 2) {
            PRINT_RESULT = atoi(argv[2]);
            if (argc > 3) {
                ITERATIONS = atoi(argv[3]);
            }
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    int local_iterations = ITERATIONS / size;
    double local_regrets[NUM_ACTIONS] = {0};
    double global_regrets[NUM_ACTIONS] = {0};

    if (PRINT_TIME) {
        start = MPI_Wtime();
    }

    cfr_iteration(local_regrets, local_iterations);

    MPI_Reduce(local_regrets, global_regrets, NUM_ACTIONS, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double strat[NUM_ACTIONS];
        get_strategy(global_regrets, strat);
        if (PRINT_TIME) {
            end = MPI_Wtime();
        }
        if (PRINT_RESULT)
            printf("Final strategy:\n  PASS: %.3f\n  BET:  %.3f\n", strat[PASS], strat[BET]);
        if (PRINT_TIME)
            printf("%f\n", end - start);
    }

    MPI_Finalize();
    return 0;
}

