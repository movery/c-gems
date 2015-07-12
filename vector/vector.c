#include "vector.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define       ARRAY(x) ((x) ->       array)
#define         CAP(x) ((x) ->    capacity)
#define     MEMBERS(x) ((x) -> num_members)
#define MEMBER_SIZE(x) ((x) -> member_size)

struct mwVector {

        void *array;
        
        size_t capacity;
        size_t num_members;

        size_t member_size;
};


int mwv_init(mwVector **v, size_t member_size)
{
        if(!v || *v)
                return -1;

        mwVector *new_vector = NULL;

        new_vector = malloc(sizeof(mwVector));

        CAP(new_vector) = 4;

        MEMBERS(new_vector) = 0;

        MEMBER_SIZE(new_vector) = member_size;

        ARRAY(new_vector) = malloc(4 * member_size);

        if(!ARRAY(new_vector))
        {
                free(new_vector);
                return -1;
        }

        *v = new_vector;
        return 0;
}

int mwv_destroy(mwVector **v)
{
        if(!v)
                return -1;

        mwVector *mwv = *v;

        free(ARRAY(mwv));

        free((void *) mwv);

        return 0;
}

int mwv_resize(mwVector *v, size_t new_size)
{
        void *new_array = realloc(ARRAY(v), new_size * MEMBER_SIZE(v));

        if(!new_array)
                return -1;

        ARRAY(v) = new_array;

        CAP(v) = new_size;

        if(new_size < MEMBERS(v))
        {
                MEMBERS(v) = new_size;
        }

        return 0;
}

int mwv_push_back(mwVector *v, void *data)
{
        if(!v)
                return -1;
        
        if(CAP(v) <= MEMBERS(v) + 1)
        {
                mwv_resize(v, CAP(v) << 1);
        }

        memcpy(ARRAY(v) + MEMBERS(v) * MEMBER_SIZE(v), 
               data, 
               MEMBER_SIZE(v));
        
        MEMBERS(v)++;

        return 0;
}

int mwv_shrink_to_fit(mwVector *v)
{
        if(!v)
                return -1;

        void * rc;
        size_t shrunk_size = MEMBERS(v) + 1;

        if(CAP(v) > shrunk_size)
        {
                rc = realloc( ARRAY(v), shrunk_size + 1);

                if(!rc)
                        return -1

                CAP(v) = shrunk_size;
        }
        return 0;
}

#undef ARRAY
#undef CAP
#undef MEMBERS
#undef MEMBER_SIZE
