--
-- Solutions to Homework #3 (SQL Queries), Fall 2015.
--

--
-- Query #1:	List all attributes of the employees from department #5.
--

select *
from employee
where dno = 5;

--
-- Query #2:	What are the SSNs of the employees working on project #10?  
--		Sort the result in descending order.
--

select essn
from works_on
where pno = 10
order by essn desc;

--
-- Query #3:	What is the Cartesian Product of the employees' first names 
--		and the dependents' names?  List each pair of names only 
--		once in the result.
--

select fname, dependent_name
from employee, dependent;

--
-- Query #4:	At which locations is the research department located?
--

select dlocation
from department, dept_locations
where department.dnumber = dept_locations.dnumber
and dname = 'Research';

--
-- Query #5:	What is the full name of the employee who has the largest 
--		salary?
--

select fname, minit, lname
from employee
where salary in ( select max(salary)
		  from employee      );

--
-- Query #6:	What are the names of the departments with employees who 
--		have a dependent named Alice?
--

select dname
from employee e, department d, dependent p
where e.ssn = p.essn
and   e.dno = d.dnumber
and   dependent_name = 'Alice';

--
-- Query #7:	Retrieve the SSNs of the employees who work in dept. 5 or 
--		who directly supervise an employee in dept. 5.
--

select ssn
from employee
where dno = 5
  union
select superssn
from employee
where dno = 5;

--
-- Query #8:	What are the birthdates of the managers of the departments
--              with projects from Stafford?
--

select distinct to_char(bdate,'DD-MON-YYYY') as "Birthdate"
from project p, department d, employee e
where mgrssn = ssn
and   dnum = dnumber
and   plocation = 'Stafford';

--
-- Query #9:	For each employee, retrieve his/her full name and the full 
--		name of his/her immediate supervisor. (Elmasri, p.181)
--              An outer join is needed to include all employees, including
--              those w/o a supervisor (that is, a superssn of NULL).
--              An ordinary join (the commented out version, below) will
--              leave out James Borg.
--

select e.fname, e.minit, e.lname, f.fname, f.minit, f.lname
  from employee e, employee f
 where e.superssn = f.ssn (+);

-- select e.fname, e.minit, e.lname, f.fname, f.minit, f.lname
--   from employee e, employee f
--  where e.superssn = f.ssn;


--
-- Query #10:	What are the names of the male dependents that are also 
--		dependent on a male employee?
--

select dependent_name
from dependent
where sex = 'M'
  intersect
select dependent_name
from dependent, employee
where essn = ssn
and   employee.sex = 'M';

--
-- Query #11:	What are the salaries of the employees from department 5 
--		who are NOT working on ProductY?
--

select ssn, salary
from employee
where dno = 5
  minus
select ssn, salary
from employee, project, works_on
where pno=pnumber
and   essn = ssn
and   pname='ProductY';

--
-- Query #12:	For each project on which more than two employees work, 
--		retrieve the project number, project name, and the number 
--		of employees who work on that project.  (Elmasri p. 192)
--

select pnumber, pname, count(*)
from   project, works_on
where  pnumber=pno
group by pnumber, pname
having count(*) > 2;

--
-- Query #13:   What are the names of the departments which employ all 
--              genders?  (In other words, the departments that have at 
--              least one woman and at least one man.)	
--

--
-- (1) Direct application of Relational Algebra definition
--

select dname from department
where  dnumber in
    ( select distinct dno from employee
      minus
      select dno from 
          ( select dno, sex 
            from   ( select dno from employee ) t1,
                   ( select sex from employee ) t2
            minus
            select dno, sex from employee
          ) t3
    );

--
-- (2) Double "not exists"
--

select dname from department
where  dnumber in
    ( select dno from employee globl
      where not exists
          ( select sex from employee
            where not exists
                ( select * from employee locl
                  where locl.sex = employee.sex and locl.dno = globl.dno
                )
          )
    );

--
-- (3) Superset
--

select dname from department
where  dnumber in
    ( select dno from employee globl
      where not exists (
          ( select sex from employee )
          minus
          ( select sex from employee
            where  employee.dno=globl.dno
          )
      )
    );

--
-- (4) Counting
--

select dname from department
where  dnumber in
    ( select dno from employee
      group by dno
      having count(distinct sex) = ( select count (distinct sex)
                                     from   employee
                                   )
    );
