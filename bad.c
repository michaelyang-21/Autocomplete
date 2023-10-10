#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#include "autocomplete.h"

int compare_terms(const void *p_a, const void *p_b)
{
    struct term *p_a_s = (struct term*)p_a;
    struct term *p_b_s = (struct term*)p_b;
    return (p_a_s->term, p_b_s->term);
}


int compare_terms_weight(const void *p_a, const void *p_b)
{
    struct term *p_a_s = (struct term*)p_a;
    struct term *p_b_s = (struct term*)p_b;
    return p_a_s->weight - p_b_s->weight;
}

void read_in_terms(term **terms, int *pnterms, char *filename) 
{
    char line[200];
    FILE *fp = fopen(filename, "r");

    fgets(line, sizeof(line), fp);
    *pnterms = atoi(line);

    *terms = (term *)malloc(*pnterms * sizeof(term));

    int term_number = 0;

    while (fgets(line, sizeof(line), fp)) 
    {
        int whitespace = 0;
        for (whitespace = 0; whitespace < strlen(line); whitespace++) 
        {
            if (isdigit(line[whitespace]))
            {
                int sum = 0;
                while (line[whitespace] != '\t')
                {
                    sum = sum * 10 + line[whitespace] -'0';
                }

                char word[100];
                whitespace += 1;
                int word_length = 0;
                while (line[whitespace])
                {
                    word[word_length] = line[whitespace];
                    word_length+=1;
                    whitespace +=1;
                }

                strcpy((*terms)[term_number].term, word);
                (*terms)[term_number].weight = sum;
                term_number += 1;
                break;
            }
        }
    }
    qsort(*terms, *pnterms, sizeof(term *), compare_terms);
    fclose(fp);
}

int lowest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    
    while (low <= high)
    {
        int mid = (low + high) / 2; // integer division
        int comparison = compare_terms(terms[mid].term, substr);
        if (comparison < 0)
        {
            low = mid + 1;
        }
        else if (comparison > 0)
        {
            high = mid -1;
        }
        else
        {
            while (mid > 0 && strcmp(terms[mid-1].term, substr))
            {
                mid --;
            }
            return mid;
        }
        
    }
    return -1;
}

int highest_match(struct term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2; // integer division
        int comparison = compare_terms(terms[mid].term, substr);
        if (comparison < 0)
        {
            low = mid + 1;
        }
        else if (comparison > 0)
        {
            high = mid -1;
        }
        else
        {
            while (mid < nterms - 1 && strcmp(terms[mid-1].term, substr))
            {
                mid --;
            }
            return mid;
        }
        
    }
    return -1;
    
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    *n_answer = last - first + 1;
    term *new = (term *)malloc(*n_answer*(sizeof(term)));

    for (int i = 0; i < n_answer; i++)
    {
        strcpy(new[i].term, terms[first + i].term);
        new[i].weight = terms[i].weight;
    }

    qsort(new, *n_answer, sizeof(term *), compare_terms_weight);
    strcpy((*answer)->term, new[*n_answer-1].term);
    (*answer)->weight = new[*n_answer-1].weight;
}
