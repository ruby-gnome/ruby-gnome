
-- SQL definitions of tables needed by sample.rb.

drop table if exists customers;
create table customers (
    ref int unsigned not null auto_increment primary key,
    name varchar(50),
    surname varchar(50),
    phone varchar(50)
);

drop table if exists accounts;
create table accounts (
    ref int unsigned not null auto_increment primary key,
    ref_customer int unsigned references customers(ref),
    balance int 
);

