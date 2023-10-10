#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;

int compare_terms(const void *p_a, const void *p_b)
{
    struct term *p_a_s = (struct term*)p_a;
    struct term *p_b_s = (struct term*)p_b;
    return strcmp(p_a_s->term, p_b_s->term);
}

int compare_terms1(const void *p_a, const void *p_b)
{
    struct term p_a_s = *(struct term*)p_a;
    struct term p_b_s = *(struct term*)p_b;
    return strcmp(p_a_s.term, p_b_s.term);
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
    FILE *fptr = fopen("C:\\Users\\Qiren\\Desktop\\ESC190\\project\\output.txt","w");

    /*if (fp == NULL)
    {
        printf("File does not exist");
        exit(EXIT_FAILURE);
    }*/

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
        
        //fprintf(fptr,"%lf ",((*terms)+line_number) -> weight);


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

        //printf("%s\n", (*terms + line_number)->term);
        //fprintf(fptr,"%lf ",(*terms + line_number) -> weight);
        //fprintf(fptr,"%s\n",(*terms + line_number) -> term);
        
        line_number += 1;
    }
    //printf("%lf ", ((*terms+2))->weight);
    //printf("%s\n", ((*terms+2))->term);
    // works up to here

    // sort terms in lexographical order
    // *terms -> term all goes to null for some reason
    // or both weight and terms go to 0.00000
    qsort(*terms, *pnterms, sizeof(term), compare_terms);

    //printf("%lf", (*terms+1)->weight);
    //printf("%s\n", (*terms+1)->term);
    
    /*int x = 0;
    while(x < *pnterms)
    {
        fprintf(fptr,"%lf ",((*terms) + x)->weight);
        fprintf(fptr,"%s\n",((*terms) + x)->term);
        x++;
    }*/

    fclose(fptr);
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


        // find the existence of substring in string
        char word[strlen(substr)];
        memset(word, '\0', sizeof(word));
        int i = 0;
        while(i < strlen(substr))
        {
            word[i] = ((terms+mid)->term)[i];
            i++;
        }

        // binary search starts
        int comparison = strcmp(word, substr);
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
            while (mid > 0)
            {
                char word[200];
                memset(word, '\0', sizeof(word));
                int j = 0;

                while(j < strlen(substr))
                {
                    word[j] = ((terms+mid)->term)[j];
                    j++;
                }

                if (strcmp(word, substr) != 0)
                {
                    break;
                }

                mid --; 
            }

            // return the smallest middle index that is matched
            //printf("%s\n", (terms+mid)->term);
            //printf("%s\n", (terms+mid+1)->term);
            return mid+1;
        }
        
    }
    // does not exist
    return -1;
}

int highest_match(term *terms, int nterms, char *substr)
{
    // initializing indexes
    int low = 0;
    int high = nterms - 1;
    
    while (low <= high)
    {
        int mid = (low + high) / 2; // integer division


        // find the existence of substring in string
        char word[strlen(substr)];
        memset(word, '\0', sizeof(word));
        int i = 0;
        while(i < strlen(substr))
        {
            word[i] = ((terms+mid)->term)[i];
            i++;
        }

        // binary search starts
        int comparison = strcmp(word, substr);
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
            while (mid < (nterms - 1))
            {
                char word[200];
                memset(word, '\0', sizeof(word));
                int j = 0;

                while(j < strlen(substr))
                {
                    word[j] = ((terms+mid)->term)[j];
                    j++;
                }

                if (strcmp(word, substr) != 0)
                {
                    break;
                }

                mid ++; 
            }

            // return the smallest middle index that is matched
            //printf("%s\n", (terms+mid-1)->term);
            //printf("%s\n", (terms+mid)->term);
            return mid-1;
        }
        
    }
    // does not exist
    return -1;
}

void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr)
{
    FILE *fptr = fopen("C:\\Users\\Qiren\\Desktop\\ESC190\\project_one\\output.txt","w");

    *answer = (term *)malloc(sizeof(term));


    int first = lowest_match(terms, nterms, substr);
    int last = highest_match(terms, nterms, substr);

    *n_answer = last - first + 1;
    term *new = (term *)malloc((*n_answer)*(sizeof(term)));

    for (int i = 0; i < *n_answer; i++)
    {
        strcpy((new+i)->term, (terms + first + i)->term);
        (new+i)->weight = (terms + first + i)->weight;

        //fprintf(fptr,"%lf ",(*(new + i)).weight);
        //fprintf(fptr,"%s\n",(*(new + i)).term);


    }

    qsort(new, *n_answer, sizeof(term), compare_terms_weight);

    int x = 0;
    while(x < *n_answer)
    {
        fprintf(fptr,"%lf ",(*(new + x)).weight);
        fprintf(fptr,"%s\n",(*(new + x)).term);
        x++;
    }

    strcpy((*answer)->term, (*new).term);
    (*answer)->weight = (*new).weight;
    //fprintf(fptr,"%lf ",(*answer)->weight);
    //fprintf(fptr,"%s\n",(*answer)->term);

    fclose(fptr);
    free(new);
}



int main()
{
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "C:\\Users\\Qiren\\Desktop\\ESC190\\project_one\\cities.txt");
    printf("number of lines in the readable file: %d\n", nterms); // works
    printf("lowest index match: %d\n", lowest_match(terms, nterms, "Tor")); // works
    printf("highest index match: %d\n", highest_match(terms, nterms, "Tor")); // works

    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Tor"); //
    printf("heighest weight and word: %lf ", answer->weight);
    printf("%s\n", answer->term);

}