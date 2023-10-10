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
    return strcmp(p_a_s->term, p_b_s->term);
}

int compare_terms_weight(const void *p_a, const void *p_b)
{
    struct term *p_a_s = (struct term*)p_a;
    struct term *p_b_s = (struct term*)p_b;

    if (p_a_s->weight == p_b_s -> weight)
    {
        return 0;
    }
    else if ((p_a_s->weight) - (p_b_s -> weight) < 0)
    {
        return 1;
    }
    else {
        return -1;
    }
}

void read_in_terms(term **terms, int *pnterms, char *filename) 
{
    char line[200];
    FILE *fp = fopen(filename, "r");

    // gets the first line (get the number of lines in the file)
    fgets(line, 200, fp);
    *pnterms = atoi(line);

    // allocate pnterms of memory with size of term
    *terms = (term *)malloc(*pnterms * sizeof(term));

    // go through each line

    int line_number = 0;
    while(fgets(line, 200, fp))
    {
        // skip each blank space until a number is reached
        int i = 0;
        while (line[i] == ' '){
            i++;
        }

        // finding the value of the number
        // by going through the line until it reaches the tab
        int w = 0;
        while (line[i] != '\t'){
            w = w*10+atoi(line[i]);
            i++;
        }

        // assign the value to the weight of the term
        terms[line_number] -> weight = (double) w;

        i+=1; // skips the tab

        // making new word
        char word[200];
        int j = 0;
        while (line[i]){
            // add every new character after the tab to word
            word[j] = line[i];
            j++;
            i++;
        }
        // assign the word to the term in struct terms
        strcpy(terms[line_number]->term, word);
        line_number += 1;
    }

    // sort terms in lexographical order
    qsort(*terms, *pnterms, sizeof(term *), compare_terms);
    fclose(fp);
}

int lowest_match(term *terms, int nterms, char *substr)
{
    // initializing indexes
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
            // compare the one just below
            while (mid > 0 && strcmp(terms[mid-1].term, substr) == 0)
            {
                mid --;
            }
            // return the smallest middle index that is matched
            return mid;
        }
        
    }
    // does not exist
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
            // compare the current and the one just above
            while (mid < nterms - 1 && strcmp(substr, terms[mid+1].term) == 0)
            {
                mid ++;
            }
            return mid;
        }
        
    }
    // the word doesn't exist
    return -1;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    *n_answer = last - first + 1;
    term *new = (term *)malloc(*n_answer*(sizeof(term)));

    for (int i = 0; i < *n_answer; i++)
    {
        strcpy(new[i].term, terms[first + i].term);
        new[i].weight = terms[i].weight;
    }

    qsort(new, *n_answer, sizeof(term *), compare_terms_weight);
    strcpy((*answer)->term, new[*n_answer-1].term);
    (*answer)->weight = new[*n_answer-1].weight;
}


