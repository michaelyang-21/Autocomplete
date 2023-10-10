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

    if (fp == NULL)
    {
        printf("File does not exist");
        exit(EXIT_FAILURE);
    }

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
        double w = 0;
        while (line[i] != '\t'){
            w = w*10 + (line[i] - '0');
            i++;
        }
        // assign the value to the weight of the term
        ((*terms)+line_number) -> weight = w;

        i += 1; // skips the tab

        // making new word
        char word[200];

        // memset
        memset(word, '\0', sizeof(word));

        int j = 0;
        while (line[i]!='\n'){
            // add every new character after the tab to word
            word[j] = line[i];
            j++;
            i++;
        }
        // assign the word to the term in struct terms
        strcpy(((*terms)+line_number)->term, word);
        
        line_number += 1;
    }

    qsort(*terms, *pnterms, sizeof(term), compare_terms);

    fclose(fp);
}

int lowest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    int res = -1;
    while (low <= high) {
        // Normal Binary Search 
        int mid = (low + high) / 2;

        char word[strlen(substr)+1];
        //memset(word, '\0', sizeof(word));
        int i = 0;
        while(i < strlen(substr))
        {
            word[i] = ((terms+mid)->term)[i];
            i++;
        }
        word[i] = '\0';

        int comparison = strcmp(word, substr);

        if (comparison > 0)
        {
            high = mid - 1;
        }
        else if (comparison < 0)
        {
            low = mid + 1;
        }
        else {
            res = mid;
            high = mid - 1;
        }
    }

    return res;
}

int highest_match(term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    int res = -1;
    while (low <= high) {
        // Normal Binary Search 
        int mid = (low + high) / 2;

        char word[strlen(substr)+1];
        //memset(word, '\0', sizeof(word));
        int i = 0;
        while(i < strlen(substr))
        {
            word[i] = ((terms+mid)->term)[i];
            i++;
        }
        word[i] = '\0';

        int comparison = strcmp(word, substr);

        if (comparison > 0)
        {
            high = mid - 1;
        }
        else if (comparison < 0)
        {
            low = mid + 1;
        }
        else {
            res = mid;
            low = mid + 1;
        }
    }

    return res;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    if (first == -1 && last == -1)
    {
        *answer = NULL;
        *n_answer = 0;
        exit(EXIT_FAILURE);
    }

    *n_answer = last - first + 1;
    *answer = (term *)malloc((*n_answer)*(sizeof(term)));


    for (int i = 0; i < *n_answer; i++)
    {
        strcpy((*answer+i)->term, (terms + first + i)->term);
        (*answer+i)->weight = (terms + first + i)->weight;

        //fprintf(fptr,"%lf ",(*(*answer + i)).weight);
        //fprintf(fptr,"%s\n",(*(*answer + i)).term);


    }

    qsort(*answer, *n_answer, sizeof(term), compare_terms_weight);

}
