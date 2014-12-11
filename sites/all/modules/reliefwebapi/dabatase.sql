CREATE TABLE `reliefweb_content_filter` (
  `reliefweb_id` int(10) unsigned NOT NULL,
  `filter_id` int(10) unsigned NOT NULL,
  UNIQUE KEY `rw_f_id` (`reliefweb_id`,`filter_id`),
  KEY `rw_id` (`reliefweb_id`),
  KEY `f_id` (`filter_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `reliefweb_content` (
  `cid` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `reliefweb_id` int(10) unsigned NOT NULL,
  `title` varchar(200) NOT NULL DEFAULT '',
  `body` varchar(1000) NOT NULL DEFAULT '',
  `url` varchar(200) NOT NULL DEFAULT '',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`cid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `reliefweb` (
  `rid` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(300) NOT NULL,
  `primary_country` varchar(500) DEFAULT NULL,
  `country` varchar(500) DEFAULT NULL,
  `organisation` varchar(500) DEFAULT NULL,
  `theme` varchar(500) DEFAULT NULL,
  `disaster_type` varchar(500) DEFAULT NULL,
  `vulnerable_groups` varchar(500) DEFAULT NULL,
  `content_format` varchar(500) DEFAULT NULL,
  `language` varchar(500) DEFAULT NULL,
  `published_date` varchar(500) DEFAULT NULL,
  `active` tinyint(2) DEFAULT '0',
  `post_num_on_list` int(10) DEFAULT '10',
  `post_num_on_single` int(10) DEFAULT '1',
  `show_thumb_on_list` tinyint(2) DEFAULT '0',
  `show_thumb_on_single` tinyint(2) DEFAULT '0',
  `show_blurb_on_list` tinyint(2) DEFAULT '0',
  `show_blurb_on_single` tinyint(2) DEFAULT '0',
  `get_blurb` tinyint(2) DEFAULT '0',
  `redirect_to_reliefweb` tinyint(2) DEFAULT '0',
  `list_view_design` enum('LIST','TILE') DEFAULT 'LIST',
  PRIMARY KEY (`rid`),
  UNIQUE KEY `rid` (`rid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;