DROP TABLE IF EXISTS `venus_redis_slowlog`;

CREATE TABLE `venus_redis_slowlog` (
	`id` BIGINT PRIMARY KEY AUTO_INCREMENT NOT NULL COMMENT '自增id',
	`entry_id` BIGINT NOT NULL DEFAULT -1 COMMENT '查询唯一id',
	`duration` BIGINT NOT NULL DEFAULT -1 COMMENT '查询花费的时间',
	`time` BIGINT NOT NULL DEFAULT -1 COMMENT '查询被执行的时间',
	`ip` VARCHAR(64) NOT NULL DEFAULT '' COMMENT 'IP地址',
	`port` int not null default -1 COMMENT '端口',
	`command` VARCHAR(256) NOT NULL DEFAULT '' COMMENT '慢查询命令',
	`create_time` timestamp not null DEFAULT '0000-00-00 00:00:00' COMMENT '记录创建时间',
	`update_time` timestamp not null DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '记录最后更新时间',
	`is_deleted` tinyint DEFAULT 0 COMMENT '软删除字段'
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;