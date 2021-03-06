-- $Id$
-- Test the miniplan tutorial
-- If you get difs in this test, please update
-- http://pub.eigenbase.org/wiki/HowToWriteAnOptimizer
-- as part of updating the .ref file.  Thanks!

create schema miniplan;
set schema 'miniplan';
set path 'miniplan';

-- register UDX we'll use to populate test data
create function ramp(n int)
returns table(i int)
language java
parameter style system defined java
no sql
external name 'class net.sf.farrago.test.FarragoTestUDR.ramp';

-- define two physical partitions with identical table definition
-- column pk:  primary key
-- column hicard:  will contain mostly distinct values
-- column locard:  will contain mostly duplicate values
create table t1(pk int not null primary key, hicard int, locard int);
create table t2(pk int not null primary key, hicard int, locard int);

-- define a logical view combining the physical partitions
create view v as select * from t1 union all select * from t2;

-- populate the first partition, manipulating the UDX output to produce the 
-- desired data patterns
insert into t1(pk,hicard,locard) 
select i,i*0.9,i*0.04 from table(ramp(1000));

-- populate the second partition (with a similar but not identical data 
-- distribution)
insert into t2(pk,hicard,locard) 
select i+1000,i*0.8,i*0.05 from table(ramp(1000));

-- make some stats on the data distribution available to the optimizer
analyze table t1 compute statistics for all columns;
analyze table t2 compute statistics for all columns;

create jar miniplan_plugin 
library 'file:${FARRAGO_HOME}/examples/miniplan/plugin/FarragoMiniplan.jar'
options(0);

alter session implementation set jar miniplan.miniplan_plugin;

!set outputformat csv

explain plan for
select * from sales.depts union all select * from sales.depts;

explain plan excluding attributes for select sum(hicard) from miniplan.v;

explain plan excluding attributes for select sum(hicard) from miniplan.v;

explain plan excluding attributes for 
select locard,sum(hicard) from miniplan.v group by locard;

explain plan excluding attributes for 
select hicard,sum(locard) from miniplan.v group by hicard;

alter session set "volcano" = true;

explain plan excluding attributes for 
select locard,sum(hicard) from miniplan.v group by locard;

explain plan excluding attributes for 
select hicard,sum(locard) from miniplan.v group by hicard;

