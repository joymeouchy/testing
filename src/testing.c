#include "/home/jmeouchy/42 cursus/temp/includes/minishell.h"

int main()
{
    t_gc_list *garbage;
    t_list  *list;
    char *string;
    char *string2;

    garbage = init_grbg_collector();
    list = init_list(garbage);
    string = ft_malloc(10, garbage);
    string2 = ft_malloc(10, garbage);
    ft_free_gc(garbage);
    return (1);
}