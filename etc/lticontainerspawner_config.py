# Configuration file for LTIContainerSpawner

#
# LTIContainerSpawner Parameters
#

#
# Volumes are mounted at /user_home_dir/projects_dir/works_dir/volumes_dir
#
spawner_use_group = True
group_home_dir    = '/home/{groupname}'
user_home_dir     = group_home_dir + '/{username}'
projects_dir      = 'jupyter'
works_dir         = 'works'
volumes_dir       = '.volumes'
notebook_dir      = user_home_dir

default_group     = 'others'                    # ホストに存在しないユーザ（ID不明）のグループ（予め作って置く）
teacher_gid       = 7000                        # 1000以上で，システムで使用していない GID
base_id           = 30000                       # ID 不明の場合に，基底となる ID番号．システムで使用されていない部分．

cpu_limit         = 1                           # Default CPU Limit
mem_limit         = 524288000                   # Default Memory Limit: 500MiB
default_url       = '/lab'
http_timeout      = 60                          # 60s


#
# PROXY
#
proxy_should_start = False                      # False means to use ltictr_proxy. True means to use configurable-http-proxy
proxy_cleanup      = True
proxy_api_url      = 'http://localhost:8001'
proxy_api_token    = "ABCDEFG"                  # Same value as LTICTR_API_Token in ltictr_proxy.conf


#
# System Environment
#
spawner_environment = {
    'GRANT_SUDO'      : 'no',                   # 通常使用では 'no'
    'PRJCT_DIR'       : projects_dir,
    'WORK_DIR'        : works_dir,
    'VOLUME_DIR'      : volumes_dir,
    'NB_UMASK'        : '0037',
    'CONDA_DIR'       : '/opt/conda',
    'CHOWN_HOME'      : 'yes',
    'CHOWN_HOME_OPTS' : '-R',
}
# CHOWN_EXTRA, CHOWN_EXTRA_OPTS

