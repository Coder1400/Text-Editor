/**
 * Machine Problem: Text Editor
 * CS 241 - Fall 2016
 */

#include "document.h"
#include "editor.h"
#include "format.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_filename(int argc, char *argv[]) {
    return argv[argc - 1];
}

void handle_display_command(Document *document, const char *command) {
    
    int size = Document_size(document);
    
    if (size <= 0){
        print_document_empty_error();
        return;
    }
    
    
    //Print entire document!
    if (strlen(command) == 1) {
        int counter = 1;
        while (counter <= size)
            print_line(document, counter++);
    }
    //print specified lines
    else{
        
        char* com = (char*)command;
        while (*com) {
            if (*(com++) == ' ')
                break;
        }
        
        int com_num = atoi(com);
        
        if (com_num > size || com_num < 1){
            invalid_line();
            return;
        }
        
        int bottom = com_num - 5;
        int top = com_num + 5;
        if (bottom < 1) bottom = 1;
        if (top > size) top = size;
        
        while (bottom <= top)
            print_line(document, bottom++);
    }

    
}

void handle_write_command(Document *document, const char *command) {

    char* com = (char*)command;
    while (*com) {
        if (*(com++) == ' ')
            break;
    }
    char* text = com;
    while (*text) {
        if (*(text++) == ' ')
            break;
    }
    
    int line_number = atoi(com);
    int doc_size = Document_size(document);
    
    size_t numtokens;
    char** split_array = strsplit(text, "$", &numtokens);
    if (numtokens == 1) {
        
        if (line_number <= doc_size)
            Document_set_line(document,line_number, text );
        else
            Document_insert_line(document,line_number, text );
    }
    else{
        
        if (line_number <= doc_size)
            Document_set_line(document,line_number, split_array[0] );
        else
            Document_insert_line(document,line_number, split_array[0] );
        
        int counter = 1;
        while (counter < (int)numtokens) {
            Document_insert_line(document,line_number + counter, split_array[counter] );
            counter++;
        }
        
        
        
    }
    
    int counter = 0;
    while (counter < (int)numtokens)
        free(split_array[counter++]);
    free(split_array);

    
    
}

void handle_append_command(Document *document, const char *command) {
    
    
    char* com = (char*)command;
    while (*com) {
        if (*(com++) == ' ')
            break;
    }
    char* text = com;
    while (*text) {
        if (*(text++) == ' ')
            break;
    }
    
    int line_number = atoi(com);
    int doc_size = Document_size(document);
    
    size_t numtokens;
    char** split_array = strsplit(text, "$", &numtokens);
    if (numtokens == 1) {
        
        if (line_number <= doc_size){
            
            const char* current = Document_get_line(document, line_number);
            int length = strlen(current) + strlen(text);
            char* new = (char*)malloc(length + 1);
            new[0] = 0;
            strcat(new, current);
            strcat(new, text);
            Document_set_line(document,line_number, new );
            free(new);
        }
        else
            Document_insert_line(document,line_number, text );
    }
    else{
        
        if (line_number <= doc_size){
            const char* current = Document_get_line(document, line_number);
            int length = strlen(current) + strlen(split_array[0]);
            char* new = (char*)malloc(length + 1);
            new[0] = 0;
            strcat(new, current);
            strcat(new, split_array[0]);
            Document_set_line(document,line_number, new );
            free(new);

        }
        else
            Document_insert_line(document,line_number, split_array[0] );
        
        int counter = 1;
        while (counter < (int)numtokens) {
            Document_insert_line(document,line_number + counter, split_array[counter] );
            counter++;
        }
        
        
        
    }
    
    int counter = 0;
    while (counter < (int)numtokens)
        free(split_array[counter++]);
    free(split_array);
    
  
}

void handle_delete_command(Document *document, const char *command) {
    
    char* com = (char*)command;
    while (*com) {
        if (*(com++) == ' ')
            break;
    }
    
    int line_number = atoi(com);
    
    if (line_number > (int)Document_size(document)){
        invalid_line();
        return;
    }
    Document_delete_line(document, line_number);
    
    
}

void handle_search_command(Document *document, const char *command) {
  
    
    
    
    if (strlen(command) < 1) {
        return;
    }
    
    char* text = (char*)command + 1;
    int counter = 1;
    size_t size = Document_size(document);
    while(counter <= (int)size){
        
        const char* string = Document_get_line(document, counter);
        
        char* substring = strstr(string, text);
        if (substring)
            print_search_line(counter, string, substring, text);
        counter++;
    }
       
}

void handle_save_command(Document *document, const char *filename) {
    Document_write_to_file(document, filename);
}
