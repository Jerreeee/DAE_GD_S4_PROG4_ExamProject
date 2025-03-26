#Add a "prefix" before each string in a list
function(AddPrefix inputList prefix)
    set(result "")
    foreach(item IN LISTS ${inputList})
        list(APPEND result "${prefix}${item}")
    endforeach()
    set(${inputList} "${result}" PARENT_SCOPE)
endfunction()