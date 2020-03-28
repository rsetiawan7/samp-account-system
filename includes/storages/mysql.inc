#if defined STORAGE_METHOD_INCLUDED
    #error "You cannot include MySQL's storage method because one of storage method already defined. Please choose ONLY one method."
#else
    #define STORAGE_METHOD_INCLUDED
#endif