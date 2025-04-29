



member(X, [X|_]).
member(X, [_|T]) :- member(X, T).


disjoint([], [_|_]).
disjoint([H1|T1],[H2|T2]) :-
	(member(H1, [H2|T2]) ->
		fail;
		disjoint(T1, [H2|T2])
	).
	









countValues(_, [], 0).
countValues(X, [H|T], N) :-
	(X =:= H ->
	countValues(X, T, N1), N is N1+1;
	countValues(X, T, N1), N is N1+0
	).
	





letter(X, Grade) :-
	Grade = a, is_of_type(positive_integer, X), X >= 90.
	

letter(X, Grade) :-
	Grade = b, is_of_type(positive_integer, X), X >= 80, X < 90.
	

letter(X, Grade) :-
	Grade = c, is_of_type(positive_integer, X), X >= 70, X < 80.


letter(X, Grade) :-
	Grade = d, is_of_type(positive_integer, X), X >= 60, X < 70.
	

letter(X, Grade) :-
	Grade = f, is_of_type(positive_integer, X), X >= 0, X < 60.
	
letter(_, Grade) :-
	Grade = unknown_grade_value.
	