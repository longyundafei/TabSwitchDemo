"""
Configuration file for CAN Message Generator System
"""

# DBC文件配置
DBC_CONFIG = {
    "dbc_path": "me11250429/ME11_CMX_BACCANFD_PPVCar_CDC_V01.13_20250408.dbc",
    "filter_diagnostic_messages": True,
    "diagnostic_id_range": (0x700, 0x7FF),  # 诊断报文ID范围
    "default_cycle_time_ms": 100,  # 默认周期时间（毫秒）
}

# 网络配置
NETWORK_CONFIG = {
    #"host": "127.0.0.1",
    "host": "0.0.0.0",
    "port": 12345,
    "max_clients": 10,
    "timeout": 30,  # 客户端超时时间（秒）
    "buffer_size": 1024,
}

# 消息生成器配置
GENERATOR_CONFIG = {
    "enable_checksum": True,
    "checksum_signal_keywords": ["chksum", "checksum", "crc"],  # 校验和信号关键字
    "update_queue_size": 100,
    "max_refresh_count": 1000000,  # 最大刷新计数
}

# 控制台配置
CONSOLE_CONFIG = {
    "prompt": "> ",
    "show_help_on_startup": True,
    "command_history_size": 100,
    "auto_complete": True,
}

# 日志配置
LOGGING_CONFIG = {
    "level": "INFO",  # DEBUG, INFO, WARNING, ERROR, CRITICAL
    "format": "%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    "log_to_file": False,
    "log_file": "can_generator.log",
    "max_log_size": 10 * 1024 * 1024,  # 10MB
    "backup_count": 5,
}

# 系统配置
SYSTEM_CONFIG = {
    "enable_performance_monitoring": False,
    "max_memory_usage_mb": 512,
    "graceful_shutdown_timeout": 10,  # 优雅关闭超时时间（秒）
    "debug_mode": False,
}

# 数据验证配置
VALIDATION_CONFIG = {
    "strict_signal_validation": True,
    "allow_unknown_signals": False,
    "validate_signal_ranges": True,
    "signal_value_precision": 6,  # 信号值精度（小数位数）
}

def get_config(section: str = None) -> dict:
    """
    获取配置信息
    
    Args:
        section: 配置节名称，如果为None则返回所有配置
        
    Returns:
        dict: 配置字典
    """
    all_config = {
        "dbc": DBC_CONFIG,
        "network": NETWORK_CONFIG,
        "generator": GENERATOR_CONFIG,
        "console": CONSOLE_CONFIG,
        "logging": LOGGING_CONFIG,
        "system": SYSTEM_CONFIG,
        "validation": VALIDATION_CONFIG,
    }
    
    if section is None:
        return all_config
    
    return all_config.get(section, {})

def update_config(section: str, key: str, value) -> bool:
    """
    更新配置值
    
    Args:
        section: 配置节名称
        key: 配置键名
        value: 新值
        
    Returns:
        bool: 更新成功返回True，否则返回False
    """
    config_map = {
        "dbc": DBC_CONFIG,
        "network": NETWORK_CONFIG,
        "generator": GENERATOR_CONFIG,
        "console": CONSOLE_CONFIG,
        "logging": LOGGING_CONFIG,
        "system": SYSTEM_CONFIG,
        "validation": VALIDATION_CONFIG,
    }
    
    if section not in config_map:
        return False
    
    if key not in config_map[section]:
        return False
    
    config_map[section][key] = value
    return True

def print_config(section: str = None) -> None:
    """
    打印配置信息
    
    Args:
        section: 配置节名称，如果为None则打印所有配置
    """
    config = get_config(section)
    
    if section:
        print(f"\n=== {section.upper()} Configuration ===")
        for key, value in config.items():
            print(f"{key}: {value}")
    else:
        print("\n=== All Configuration ===")
        for sect_name, sect_config in config.items():
            print(f"\n[{sect_name.upper()}]")
            for key, value in sect_config.items():
                print(f"  {key}: {value}")
    print()

def validate_config() -> list:
    """
    验证配置有效性
    
    Returns:
        list: 验证错误列表，如果为空则配置有效
    """
    errors = []
    
    # 验证DBC配置
    if not DBC_CONFIG["dbc_path"]:
        errors.append("DBC path cannot be empty")
    
    if DBC_CONFIG["default_cycle_time_ms"] <= 0:
        errors.append("Default cycle time must be positive")
    
    # 验证网络配置
    if not (1 <= NETWORK_CONFIG["port"] <= 65535):
        errors.append("Port must be between 1 and 65535")
    
    if NETWORK_CONFIG["max_clients"] <= 0:
        errors.append("Max clients must be positive")
    
    if NETWORK_CONFIG["timeout"] <= 0:
        errors.append("Timeout must be positive")
    
    # 验证生成器配置
    if GENERATOR_CONFIG["update_queue_size"] <= 0:
        errors.append("Update queue size must be positive")
    
    if GENERATOR_CONFIG["max_refresh_count"] <= 0:
        errors.append("Max refresh count must be positive")
    
    # 验证系统配置
    if SYSTEM_CONFIG["max_memory_usage_mb"] <= 0:
        errors.append("Max memory usage must be positive")
    
    if SYSTEM_CONFIG["graceful_shutdown_timeout"] <= 0:
        errors.append("Graceful shutdown timeout must be positive")
    
    # 验证数据验证配置
    if not (0 <= VALIDATION_CONFIG["signal_value_precision"] <= 10):
        errors.append("Signal value precision must be between 0 and 10")
    
    return errors

# 在模块导入时验证配置
if __name__ == "__main__":
    # 如果作为主程序运行，打印配置信息
    print_config()
    
    # 验证配置
    errors = validate_config()
    if errors:
        print("Configuration validation errors:")
        for error in errors:
            print(f"  - {error}")
    else:
        print("Configuration validation passed.")
else:
    # 作为模块导入时，验证配置
    validation_errors = validate_config()
    if validation_errors:
        print("Warning: Configuration validation failed:")
        for error in validation_errors:
            print(f"  - {error}")
