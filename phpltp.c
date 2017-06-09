
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_phpltp.h"

#include "ltps.h"



/* If you declare any globals in php_phpltp.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(phpltp)
*/

/* True global resources - no need for thread safety here */
static int le_phpltp;
static int le_phpltp_persist;
#define PHP_PHPLTP_RES_NAME "PHPLTP resource"

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("phpltp.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_phpltp_globals, phpltp_globals)
    STD_PHP_INI_ENTRY("phpltp.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_phpltp_globals, phpltp_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_phpltp_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_phpltp_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "phpltp", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(segmentor_create_segmentor)
{
    char *path = NULL;
    size_t path_len;
    char *lexicon_path = NULL;
    size_t lexicon_path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|s", &path, &path_len, &lexicon_path, &lexicon_path_len) == FAILURE) {
        return;
    }
    void * engine = c_segmentor_create_segmentor(path,lexicon_path);
    RETURN_RES(zend_register_resource(engine, le_phpltp));
}

PHP_FUNCTION(segmentor_release_segmentor)
{
    zval * zengine;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zengine) == FAILURE)
    {
        RETURN_FALSE;
    }
    void * engine;
    if((engine = (void *)zend_fetch_resource2(Z_RES_P(zengine), PHP_PHPLTP_RES_NAME, le_phpltp, le_phpltp_persist)) == NULL)
    {
        RETURN_FALSE;
    }
    int ret = c_segmentor_release_segmentor(engine);
    if( ret == 0)
    {
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_FUNCTION(segmentor_segment)
{
    zval * zengine;
    char *text = NULL;
    size_t text_len;
    zval *zwords;
    size_t zwords_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs|a", &zengine, &text, &text_len, &zwords, &zwords_len) == FAILURE)
    {
        RETURN_FALSE;
    }
    if(text_len<2)
    {
        RETURN_LONG(text_len);
    }
    void * engine;
    if((engine = (void *)zend_fetch_resource2(Z_RES_P(zengine), PHP_PHPLTP_RES_NAME, le_phpltp, le_phpltp_persist)) == NULL)
    {
        RETURN_FALSE;
    }
    const char** words;
    int len = c_segmentor_segment(engine, text, &words);
    //printf("%d", len);
    //array_init(zwords);//引用传递，无需再次初始化
    int i = 0;
    for(;i<len;i++)
    {
        //printf("%s ", words[i]);
        add_next_index_string(zwords, words[i]);
        //c_words_free(words);
    }
    c_words_free(words,len);
    RETURN_LONG(len);
/*
    array_init(return_value);
    add_next_index_null(return_value);
    add_next_index_long(return_value, 42);
    add_next_index_bool(return_value, 1);
    add_next_index_double(return_value, 3.14);
    add_next_index_string(return_value, "foo");
    add_assoc_string(return_value, "mno", "baz");
    add_assoc_bool(return_value, "ghi", 1);
    RETURN_ZVAL(return_value,1,1);
*/
}

static void phpltp_dtor(zend_resource *rsrc TSRMLS_DC)
{
    return;
}

/* {{{ php_phpltp_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_phpltp_init_globals(zend_phpltp_globals *phpltp_globals)
{
	phpltp_globals->global_value = 0;
	phpltp_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(phpltp)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
        le_phpltp = zend_register_list_destructors_ex(phpltp_dtor, NULL, PHP_PHPLTP_RES_NAME, module_number);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(phpltp)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(phpltp)
{
#if defined(COMPILE_DL_PHPLTP) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(phpltp)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(phpltp)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "phpltp support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ phpltp_functions[]
 *
 * Every user visible function must have an entry in phpltp_functions[].
 */
const zend_function_entry phpltp_functions[] = {
	PHP_FE(confirm_phpltp_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(segmentor_create_segmentor, NULL)
	PHP_FE(segmentor_release_segmentor, NULL)
	PHP_FE(segmentor_segment, NULL)
	PHP_FE_END	/* Must be the last line in phpltp_functions[] */
};
/* }}} */

/* {{{ phpltp_module_entry
 */
zend_module_entry phpltp_module_entry = {
	STANDARD_MODULE_HEADER,
	"phpltp",
	phpltp_functions,
	PHP_MINIT(phpltp),
	PHP_MSHUTDOWN(phpltp),
	PHP_RINIT(phpltp),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(phpltp),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(phpltp),
	PHP_PHPLTP_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PHPLTP
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(phpltp)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
