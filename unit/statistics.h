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

static int compare_uint64t(const void *arg1, const void *arg2){
    const uint64_t val1 = *(uint64_t *) arg1, val2 = *(uint64_t *) arg2;

    return val1 - val2;
}

static inline void evaluate_statistics(const struct statistics_values *stats, const char *benchmark_name){
    uint64_t sum = 0, min, max;
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

    qsort(stats->samples_ns,stats->number_samples,sizeof(uint64_t),compare_uint64t);
    min = stats->samples_ns[0];
    max = stats->samples_ns[stats->number_samples - 1];
    fprintf(stderr, "##########################################\n");
    fprintf(stderr, "Collected %"PRIu64" samples\n", stats->number_samples);
    fprintf(stderr, "Min %"PRIu64" ns max %"PRIu64" ns\n", min,max);
    fprintf(stderr, "Average %f ns stdev %f ns\n", average, stdev);
    fprintf(stderr, "##########################################\n");
}

static inline void destroy_stat(struct statistics_values *stat){
    if(stat->samples_ns){
        free(stat->samples_ns);
    }
    memset(stat,0,sizeof(*stat));
}
#define NS_PER_S 1000000000
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
                                        * NS_PER_S + (end_time.tv_nsec - start_time.tv_nsec); \
                                    add_sample(&stat_name,duration_ns);                         \
                            }
#define DESTROY_STAT(stat_name) destroy_stat(&stat_name);

#endif