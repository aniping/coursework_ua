-- Q1
select splname from mccann.supplier;

-- Q2
select empfname from mccann.employee 
where departname='Marketing';

-- Q3
select splname, deptname from mccann.supplier, mccann.department;

-- Q4
select distinct itemno from mccann.sale, mccann.department
where deptfloor=2 and dname=deptname order by itemno;

-- Q5
select distinct itemno from mccann.sale
where dname in (select deptname from mccann.department
      	        where deptfloor=2) order by itemno;

-- Q6-V1
select empno, empfname from mccann.employee e1
where empsalary > (select empsalary from mccann.employee e2
      		   where e1.bossno=e2.empno);

-- Q6-V2
select e1.empno, e1.empfname from mccann.employee e1, mccann.employee e2
where e1.empsalary > e2.empsalary and e1.bossno=e2.empno;

-- Q7
select distinct itemnum from mccann.delivery, mccann.supplier
where mccann.supplier.splname='Nepalese_Corp' and
      mccann.delivery.splno=mccann.supplier.splno
union
select distinct itemno from mccann.sale
where mccann.sale.dname='Navigation';

-- Q8
select distinct itemname from mccann.item, mccann.sale, mccann.department
where deptfloor != 2 and dname=deptname and mccann.sale.itemno=mccann.item.itemno;

-- Q9-V1
select distinct itemname from mccann.item, mccann.sale, mccann.department
where dname=deptname and mccann.sale.itemno=mccann.item.itemno
minus
select distinct itemname from mccann.item, mccann.sale, mccann.department
where deptfloor=2 and dname=deptname and mccann.sale.itemno=mccann.item.itemno;

-- Q9-V2
select distinct itemname from mccann.item, mccann.sale, mccann.department
where dname=deptname and mccann.sale.itemno=mccann.item.itemno
and mccann.sale.itemno not in (select itemno from mccann.department, mccann.sale
      	 	       	       where deptfloor=2 and dname=deptname);

-- Q10
select empfname, empsalary from mccann.employee
where empno in (select bossno from mccann.employee
      	        group by bossno having count(empno) > 2);

-- Q11
select distinct splname from mccann.supplier, mccann.delivery, mccann.item
where itemname='Pith_helmet' and mccann.supplier.splno=mccann.delivery.splno 
and mccann.item.itemno=mccann.delivery.itemnum 
and mccann.delivery.dptname in (select dname from mccann.sale, mccann.department, mccann.employee
    			        where empfname='Andrew' and dname=deptname and 
				mccann.department.empno=mccann.employee.empno);

-- Q12
select deptname, avg(empsalary) from mccann.department, mccann.employee
where deptname=departname and deptfloor=2 group by deptname;

-- Q13-V1: set cardinality
select distinct itemno from mccann.sale, mccann.department
where deptfloor=2 and deptname=dname 
group by itemno having count(distinct deptname) = 
(select count(deptname) from mccann.department
 where deptfloor=2);

-- Q13-V2: double not exists: 
-- find items such that for every department on the 2nd floor, there exists
-- items that were sold by all of them;
-- <==>
-- find items such that there don't exist departments on the 2nd floor that
-- there don't exist items that were sold by all of them;
select distinct itemno from mccann.sale global_sale
where not exists 
      (select deptname from mccann.department
       where deptfloor=2 and not exists 
       	     (select * from mccann.sale local_sale
	      where local_sale.dname=mccann.department.deptname
	      and local_sale.itemno=global_sale.itemno));

-- Q13-V3: set containment
-- if an item was sold by a set of departments which is a superset of
-- the set of departments on the 2nd floor, then this item is an answer.
select distinct itemno from mccann.sale global_sale
where not exists (select deptname from mccann.department
      	  	  where deptfloor=2
		  minus
		  select dname from mccann.sale local_sale
		  where local_sale.itemno=global_sale.itemno);