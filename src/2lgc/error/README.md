# Error

# Show

Print various error message based on the categorie of the error.
  - [BUGPARAM](\ref BUGPARAM) : show an error message if a parameter is invalid, like null pointer if parameter can't be null.
  - [BUGLIB](\ref BUGLIB) : show an error message if a library is returning an error value. But it's not because of user value.
  - [BUGCRIT](\ref BUGCRIT) : show an error message if a function should not fail and the others BUG functions is not revelant.
  - [BUGUSER](\ref BUGUSER) : show an error message if function fails because of the user parameter. So it's not really a bug.
  - [BUGPROG](\ref BUGPROG) : show an error message because of the programmation error like value missing in switch/case.
  - [BUGCONT](\ref BUGCONT) : throw error.
