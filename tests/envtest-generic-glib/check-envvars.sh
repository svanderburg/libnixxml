#!/bin/sh -e

if test $string_value != "hello"
then
    echo "string_value should be: 'hello', instead it is: '$string_value'" >&2
    false
fi

if test "$int_value" != 3
then
    echo "int_value should be: '3', instead it is: '$int_value'" >&2
    false
fi

if test "$float_value" != 1.2345
then
    echo "float_value should be: '1.2345', instead it is: '$float_value'" >&2
    false
fi

if test "$bool_value" != 1
then
    echo "bool_value should be: '1', instead it is: '$bool_value'" >&2
    false
fi

if test "$list_value" != "hello 3"
then
    echo "list_value should be: 'hello 3', instead it is: '$list_value'" >&2
    false
fi
