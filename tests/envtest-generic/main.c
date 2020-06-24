#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "nixxml-node.h"
#include "nixxml-ds.h"
#include "nixxml-generate-env.h"
#include "nixxml-types.h"

#define TRUE 1

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
    xmlHashTablePtr hash_table;

    NixXML_Node node_string_value = { NIX_XML_TYPE_STRING, (void*) "hello" };
    NixXML_Node node_int_value = { NIX_XML_TYPE_INT, (void*) "3" };
    NixXML_Node node_float_value = { NIX_XML_TYPE_FLOAT, (void*) "1.2345" };
    NixXML_Node node_bool_value = { NIX_XML_TYPE_BOOL, (void*) "true" };
    NixXML_Node *list_members[3];
    NixXML_Node node_list_value;

    list_members[0] = &node_string_value;
    list_members[1] = &node_int_value;
    list_members[2] = NULL;

    node_list_value.type = NIX_XML_TYPE_LIST;
    node_list_value.value = list_members;

    /* Construct a hash table and add nodes to it */
    hash_table = xmlHashCreate(5);

    xmlHashAddEntry(hash_table, (xmlChar*) "string_value", &node_string_value);
    xmlHashAddEntry(hash_table, (xmlChar*) "int_value", &node_int_value);
    xmlHashAddEntry(hash_table, (xmlChar*) "float_value", &node_float_value);
    xmlHashAddEntry(hash_table, (xmlChar*) "bool_value", &node_bool_value);
    xmlHashAddEntry(hash_table, (xmlChar*) "list_value", &node_list_value);

    /* Compose an environment variables array from the hash table */
    envvars = NixXML_generate_env_vars_generic_ds(hash_table);

    /* Executes an external process that consumes and checks the environment variables */
    exit_status = fork_and_check_env_variables(envvars);

    /* Cleanup */
    NixXML_delete_env_variable_array(envvars);
    xmlHashFree(hash_table, NULL);

    return exit_status;
}
