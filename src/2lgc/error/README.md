# Error

## show.h

Print various error message based on the categorie of the error.
  - BUGPARAM : show an error message if a parameter is invalid, like null pointer if parameter can't be null.
  - BUGLIB : show an error message if a library is returning an error value. But it's not because of user value.
  - BUGCRIT : show an error message if a function should not fail and the others BUG functions is not revelant.
  - BUGUSER : show an error message if function fails because of the user parameter. So it's not really a bug.
  - BUGPROG : show an error message because of the programmation error like value missing in switch/case.
  - BUGCONT : throw error.
