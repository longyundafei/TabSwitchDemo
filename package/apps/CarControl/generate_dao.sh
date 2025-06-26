#!/bin/bash
set -e

# 1. 定义路径
PROJECT_DIR=$PWD
GENERATOR_DIR=$PROJECT_DIR/../../../tools/greendao-generator
GENERATOR_JAR=$GENERATOR_DIR/greendao-generator.jar
FREEMARKER_JAR=$GENERATOR_DIR/freemarker.jar
GENERATOR_CLASS=greendao_generator
GENERATOR_SRC=greendao_generator.java

# 2. 清理旧文件
echo "[GreenDAO] Cleaning old DAO files..."
rm -rf "$PROJECT_DIR/src/java/com/bihu/carcontrol/common/greendao/db"

# 3. 编译 generator
echo "[GreenDAO] Compiling generator..."
cd "$GENERATOR_DIR"
javac -cp "$GENERATOR_JAR:$FREEMARKER_JAR" "$GENERATOR_SRC"

# 4. 执行 generator（注意使用相对路径）
echo "[GreenDAO] Generating GreenDAO code into CarControl/src/java"
java -cp .:"$GENERATOR_JAR":"$FREEMARKER_JAR" "$GENERATOR_CLASS" "../../packages/apps/CarControl/src/java"

# 5. 返回原路径
cd -

