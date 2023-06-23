#ifndef STATISTICS_H
#define STATISTICS_H
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSUMED_NUMBER_SAMPLES 10000

struct statistics_values{
    uint64_t *samples_ns;
    size_t number_samples;
    size_t array_size;
};
static inline void check_alloc(const struct statistics_values *stats){
    if(!stats->samples_ns){
        perror("Could not allocate memory for array");
        abort();
    }
}
static inline void init_statistics(struct statistics_values *stats){
    stats->samples_ns=(uint64_t *)calloc(ASSUMED_NUMBER_SAMPLES,sizeof(uint64_t));
    stats->number_samples = 0;
    stats->array_size = ASSUMED_NUMBER_SAMPLES;
    check_alloc(stats);
}

static inline void add_sample(struct statistics_values *stats, uint64_t sample){
    if(stats->number_samples >= stats->array_size){
        stats->array_size *= 2;
        stats->samples_ns = (uint64_t *)realloc(stats->samples_ns,stats->array_size * sizeof(uint64_t));
        check_alloc(stats);
    }
    stats->samples_ns[stats->number_samples++] = sample;
}

static inline void evaluate_statistics(const struct statistics_values *stats, const char *benchmark_name){
    uint64_t sum = 0;
    double sum_of_squares = 0;
    double average, variance, stdev;

    if(!stats->number_samples){
        fprintf(stderr,"No samples - no evaluation!");
        return;
    }

    for(size_t i = 0; i < stats->number_samples; i++){
        sum += stats->samples_ns[i];
    }
    average = (double) sum / (double) stats->number_samples;
    for(size_t i = 0; i < stats->number_samples; i++){
        sum_of_squares += pow((average - (double) stats->samples_ns[i]),2);
    }
    variance = sum_of_squares / (double) stats->number_samples;

    stdev = sqrt(variance);
    fprintf(stderr, "##########################################\n");
    fprintf(stderr, "Average %f ns stdev %f ns\n", average, stdev);
    fprintf(stderr, "##########################################\n");
}

static inline void destroy_stat(struct statistics_values *stat){
    if(stat->samples_ns){
        free(stat->samples_ns);
    }
    memset(stat,0,sizeof(*stat));
}

#define INIT_STAT(stat_name) struct statistics_values stat_name; \
                             init_statistics(&stat_name)
#define BENCHMARK_OP(stat_name, op)                                                             \
                            {                                                                   \
                                    struct timespec start_time, end_time;                       \
                                    uint64_t duration_ns;                                       \
	                                assert(clock_gettime(CLOCK_MONOTONIC_RAW,&start_time)==0);  \
                                    op;                                                         \
                                    assert(clock_gettime(CLOCK_MONOTONIC_RAW,&end_time)==0);    \
                                    duration_ns = (end_time.tv_sec - start_time.tv_sec)         \
                                        * 1000 + (end_time.tv_nsec - start_time.tv_nsec)        \
                                        / 1000000;                                              \
                                    add_sample(&stat_name,duration_ns);                         \
                            }
#define DESTROY_STAT(stat_name) destroy_stat(&stat_name);

#endif