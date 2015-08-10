drop table if exists run;
drop table if exists study;
drop table if exists batch;

CREATE TABLE run (
	id INT primary key not null,
	study int not null,
	batch int,
	name TEXT,
	date_time TEXT,
	scenario TEXT,
	notes TEXT,
	FOREIGN KEY(study) REFERENCES study(id),
	FOREIGN KEY(batch) REFERENCES batch(id)
	);
	
CREATE TABLE study(
	id int key not null,
	name TEXT,
	default_scenario TEXT,
	date_time TEXT,
	notes TEXT
);

CREATE TABLE batch(
	id int key not null,
	parameter_name TEXT,
	parameter_value TEXT
);