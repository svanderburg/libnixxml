#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "nixxml-node.h"
#include "nixxml-glib.h"
#include "nixxml-generate-env.h"
#include "nixxml-types.h"

static NixXML_bool fork_and_check_env_variables(xmlChar **envvars)
{
    pid_t pid;
    int wstatus;

    if((pid = fork()) == 0)
    {
        char *const argv[] = { "check-envvars.sh", NULL };
        execve(argv[0], argv, (char**)envvars);
        _exit(1);
    }

    waitpid(pid, &wstatus, 0);

    return (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0);
}

int main(int argc, char *argv[])
{
    xmlChar **envvars;
    int exit_status;

    /* Compose a hash table with test data */
    GHashTable *hash_table;

    NixXML_Node node_string_value = { NIX_XML_TYPE_STRING, (void*) "hello" };
    NixXML_Node node_int_value = { NIX_XML_TYPE_INT, "3" };
    NixXML_Node node_float_value = { NIX_XML_TYPE_FLOAT, "1.2345" };
    NixXML_Node node_bool_value = { NIX_XML_TYPE_BOOL, "true" };
    GPtrArray *list_members = g_ptr_array_new();
    NixXML_Node node_list_value;

    g_ptr_array_add(list_members, &node_string_value);
    g_ptr_array_add(list_members, &node_int_value);

    node_list_value.type = NIX_XML_TYPE_LIST;
    node_list_value.value = list_members;

    /* Construct a hash table and add nodes to it */
    hash_table = g_hash_table_new(g_str_hash, g_str_equal);

    g_hash_table_insert(hash_table, (gpointer) "string_value", &node_string_value);
    g_hash_table_insert(hash_table, (gpointer) "int_value", &node_int_value);
    g_hash_table_insert(hash_table, (gpointer) "float_value", &node_float_value);
    g_hash_table_insert(hash_table, (gpointer) "bool_value",  &node_bool_value);
    g_hash_table_insert(hash_table, (gpointer) "list_value", &node_list_value);

    /* Compose an environment variables array from the hash table */
    envvars = NixXML_generate_env_vars_generic_glib(hash_table);

    /* Executes an external process that consumes and checks the environment variables */
    exit_status = fork_and_check_env_variables(envvars);

    /* Cleanup */
    g_ptr_array_free(list_members, TRUE);
    NixXML_delete_env_variable_array(envvars);
    g_hash_table_destroy(hash_table);

    return exit_status;
}
