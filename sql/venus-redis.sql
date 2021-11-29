DROP TABLE IF EXISTS `venus_redis_slowlog`;

CREATE TABLE `venus_redis_slowlog` (
	`id` BIGINT PRIMARY KEY AUTO_INCREMENT NOT NULL COMMENT '自增id',
	`entry_id` BIGINT NOT NULL DEFAULT -1 COMMENT '查询唯一id',
	`msg_queue_id` BIGINT NOT NULL DEFAULT -1 COMMENT '消息队列id',
	`duration` BIGINT NOT NULL DEFAULT -1 COMMENT '查询花费的时间',
	`time` BIGINT NOT NULL DEFAULT -1 COMMENT '查询被执行的时间',
	`ip` VARCHAR(64) NOT NULL DEFAULT '' COMMENT '客户端的IP地址',
	`port` INT NOT NULL DEFAULT -1 COMMENT '客户端的端口',
	`command` VARCHAR(256) NOT NULL DEFAULT '' COMMENT '慢查询命令',
	`redis_host` VARCHAR(256) NOT NULL DEFAULT '' COMMENT 'Redis的ip地址',
	`redis_port` int not null default -1 COMMENT 'Redis的端口',
	`redis_run_id` VARCHAR(256) NOT NULL DEFAULT '' COMMENT 'Redis的实力id',
	`redis_cluster_name` VARCHAR(256) NOT NULL DEFAULT '' COMMENT 'Redis cluster名字',
	`create_time` timestamp not null DEFAULT '0000-00-00 00:00:00' COMMENT '记录创建时间',
	`update_time` timestamp not null DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
	`is_deleted` tinyint DEFAULT 0 COMMENT '软删除字段'
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `venus_redis_cluster_instance`;

CREATE TABLE `venus_redis_cluster_instance` (
	`id` BIGINT PRIMARY KEY AUTO_INCREMENT NOT NULL COMMENT '自增id',
	`cluster_name` VARCHAR(256) NOT NULL DEFAULT '' COMMENT 'redis集群名字，逻辑名',
	`instance_host` VARCHAR(64) NOT NULL DEFAULT '' COMMENT 'redis实例IP',
	`instance_port` INT NOT NULL DEFAULT -1 COMMENT '端口',
	`create_time` timestamp not null DEFAULT '0000-00-00 00:00:00' COMMENT '记录创建时间',
	`update_time` timestamp not null DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
	`is_deleted` tinyint DEFAULT 0 COMMENT '软删除字段'
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

