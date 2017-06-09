#ifndef _MYFUNCTION_H  

#define _MYFUNCTION_H  
   
#ifdef _cplusplus  
extern "C" {  
#endif  
    void myprint();  
    void *c_segmentor_create_segmentor(const char *path, const char *lexicon_path);
    int c_segmentor_release_segmentor(void *segmentor);
    int c_segmentor_segment(void *segmentor, const char *line, const char ***words);
    int c_words_free(const char **words, int len);
#ifdef _cplusplus  
}  
#endif  

#endif  
