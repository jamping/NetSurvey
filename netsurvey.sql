
--netsurvey_user
create table netsurvey_user
(
user_name        varchar(20) primary key,
user_passwd     varchar(40),
user_level          int,
user_desc          varchar(60),
create_time        datetime
)


--netsurvey_log
create table netsurvey_log
(
log_id              varchar(40) primary key,
log_time          datetime,
log_source      varchar(40),
log_type          int,
log_message   varchar(300),
log_owner        varchar(40)
)


--netsurvey_army
create table netsurvey_army
(
army_id    varchar(40) primary key,
parent_id    varchar(40),
army_name  varchar(255) null,
is_jianzhi   int,
army_level int
)



--netsurvey_zgw
create table netsurvey_zgw
(
zgw_id  		varchar(40) primary key,
army_id  		varchar(40), 
is_connected          int,
is_fibre                     int,
is_onlyone                int,
zgw_ip                       varchar(255),
zgw_bw                     int,
zgw_computer           int,
zgw_server                int,
zgw_admin                 int,
zgw_room                  int,
zgw_website              varchar(255),
constraint netsurvey_zgw_FK1 foreign key (army_id) references netsurvey_army
)

--netsurvey_lw
create table netsurvey_lw
(
lw_id  		varchar(40) primary key,
army_id  		varchar(40), 
is_have                    int,
is_onlyone                int,
is_good                    int,
bad_type                  int,
lw_computer             int,
lw_server                  int,
lw_admin                   int,
lw_room                    int,
lw_num                      int,
lw_newnum                   int,
constraint netsurvey_lw_FK1 foreign key (army_id) references netsurvey_army
)


create table netsurvey_subzgw_stat
(
army_id  		varchar(40)  primary key,
not_conn_total          int,
fibre_total                  int,
computer_total          int,
server_total                int,
admin_total                int
)

create table netsurvey_subzgw_level_stat
(
army_id                          varchar(40),
army_level                      int,
jianzhi_total                    int,
jianzhi_conn_total          int,
feijianzhi_total                int,
feijianzhi_conn_total     int,
primary key(army_id,army_level)
)

create table netsurvey_sublw_stat
(
army_id  		varchar(40)  primary key,
had_total                    int,
good_total                 int,
new_total                   int,
bad_total                   int,
bad_all_total             int,
bad_tianxian_total   int,
bad_zhijia_total        int,
bad_gpt_total           int,
bad_tsk_total            int,
computer_total          int,
server_total                int,
admin_total                int
)