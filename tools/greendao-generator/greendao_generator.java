import org.greenrobot.greendao.generator.Entity;
import org.greenrobot.greendao.generator.Schema;
import org.greenrobot.greendao.generator.DaoGenerator;

public class greendao_generator {
    public static void main(String[] args) throws Exception {
        Schema schema = new Schema(1, "com.bihu.carcontrol.common.greendao.db");

        // RadioBean实体定义
        Entity radio = schema.addEntity("RadioBean");
        radio.addIdProperty();
        radio.addStringProperty("name");
        radio.addIntProperty("frequency");

        // MaintainBean实体定义，示例字段，根据实际需求调整
        Entity maintainBean = schema.addEntity("MaintainBean");
        maintainBean.addIdProperty();
        maintainBean.addStringProperty("description");
        maintainBean.addLongProperty("timestamp");

        // 添加缺失的字段，避免编译时找不到符号
        maintainBean.addBooleanProperty("isMaintainTimeSet");
        maintainBean.addLongProperty("maintainTimeSetTime");
        maintainBean.addIntProperty("maintainTimeSetValue");

        maintainBean.addBooleanProperty("isMaintainMileageSet");
        maintainBean.addLongProperty("maintainMileageSetTime");
        maintainBean.addIntProperty("maintainMileageSetValue");

        // 根据需要继续添加其他字段...

        // 生成代码，输出目录由参数传入
        new DaoGenerator().generateAll(schema, args[0]);
    }
}

