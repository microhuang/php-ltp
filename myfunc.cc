#include <string.h>
#include <iostream>
#include <string>
#include <malloc.h>
#include "segment_dll.h"

//#include "myclass.h"
#ifndef _cplusplus  
#define _cplusplus  
#include "myfunc.h"
#endif

//using namespace std;

/*
void myprint()  
{  
    MyClass mc;  
    mc.print();  
}  
*/

void *c_segmentor_create_segmentor(const char *path, const char *lexicon_path)
{
    void * engine = segmentor_create_segmentor(path,lexicon_path);
    //debug
    //std::cout << path << lexicon_path << std::endl;
    return engine;
}

int c_segmentor_release_segmentor(void *segmentor)
{
    int ret = segmentor_release_segmentor(segmentor);
    return ret;
}

int c_segmentor_segment(void *segmentor, const char *line, const char ***words)
{
    int len = -1;
    const char** ws = NULL;
    {
        std::vector<std::string> words;
        len = segmentor_segment(segmentor, line, words);
        ws = (const char**)malloc(sizeof(char*)*len);
        for(int i=0;i<len;i++)
        {
            char* w=(char*)malloc(sizeof(char)*strlen(words[i].data()));
            strcpy(w,words[i].data());
            ws[i]=w;
        }
        for(int i=0;i<len;i++)
        {
            printf("%s ",ws[i]);
        }
    }
    *words=ws;
    return len;
}

int c_words_free(const char **words,int len)
{
    for(int i=0;i<len;i++)
    {
        free((char*)words[i]);
    }
    free(words);
    return 0;
}

