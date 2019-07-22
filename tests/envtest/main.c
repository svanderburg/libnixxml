#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "nixxml-node.h"
#include "nixxml-ds.h"
#include "nixxml-generate-env.h"

#define TRUE 1

static int fork_and_check_env_variables(xmlChar **envvars)
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

    int int_value = 3;
    float float_value = 1.2345;
    int bool_value = TRUE;

    NixXML_Node node_string_value = { NIX_XML_TYPE_STRING, (void*) "hello" };
    NixXML_Node node_int_value;
    NixXML_Node node_float_value;
    NixXML_Node node_bool_value;
    NixXML_Node *list_members[3];
    NixXML_Node node_list_value;

    node_int_value.type = NIX_XML_TYPE_INT;
    node_int_value.value = &int_value;

    node_float_value.type = NIX_XML_TYPE_FLOAT;
    node_float_value.value = &float_value;

    node_bool_value.type = NIX_XML_TYPE_BOOL;
    node_bool_value.value = &bool_value;

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
