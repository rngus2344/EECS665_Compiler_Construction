set breakpoint pending on
set confirm off
file ./cshantyc
break cshanty::Err::report
commands
	where
end
break cshanty::InternalError::InternalError
commands
	where
end

define p4
  set args p4_tests/$arg0.cshanty -n
  run
end
