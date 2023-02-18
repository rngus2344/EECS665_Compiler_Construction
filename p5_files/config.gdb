set breakpoint pending on
set confirm off
file ./cmmc
break cminusminus::Err::report
commands
	where
end
break cminusminus::InternalError::InternalError
commands
	where
end
break cminusminus::ToDoError::ToDoError
commands
	where
end

define p5
  set args p5_tests/$arg0.cmm -c
  run
end
