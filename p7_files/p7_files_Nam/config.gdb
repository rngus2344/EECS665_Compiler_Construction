set breakpoint pending on
set confirm off
file ./cminusminusc
break cminusminus::Err::report
commands
	where
end
break cminusminus::ToDoError::ToDoError
commands
	where
end
break cminusminus::InternalError::InternalError
commands
	where
end

define t7
  set args p6_tests/$arg0.cminusminus -o --
  run
end
