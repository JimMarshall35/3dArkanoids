#include "SerializableCamera.h"

SerializableCamera::SerializableCamera(std::string path)
    :m_camera(path)
{
    LoadFromFile(path);
}


#define NUM_CAMERA_SERIALIZABLE_PROPERTIES 10

const std::vector<SerializableProperty>& SerializableCamera::GetSerializableProperties()
{
    static std::vector<SerializableProperty> props(NUM_CAMERA_SERIALIZABLE_PROPERTIES);

    props[0].name = "Position";
    props[0].type = SerializablePropertyType::Vec3;
    props[0].data.dataUnion.Vec3 = m_camera.Position;

    props[1].name = "Front";
    props[1].type = SerializablePropertyType::Vec3;
    props[1].data.dataUnion.Vec3 = m_camera.Front;

    props[2].name = "Up";
    props[2].type = SerializablePropertyType::Vec3;
    props[2].data.dataUnion.Vec3 = m_camera.Up;

    props[3].name = "Right";
    props[3].type = SerializablePropertyType::Vec3;
    props[3].data.dataUnion.Vec3 = m_camera.Right;

    props[4].name = "WorldUp";
    props[4].type = SerializablePropertyType::Vec3;
    props[4].data.dataUnion.Vec3 = m_camera.WorldUp;

    props[5].name = "Yaw";
    props[5].type = SerializablePropertyType::Float;
    props[5].data.dataUnion.Float = m_camera.Yaw;

    props[6].name = "Pitch";
    props[6].type = SerializablePropertyType::Float;
    props[6].data.dataUnion.Float = m_camera.Pitch;

    props[7].name = "MovementSpeed";
    props[7].type = SerializablePropertyType::Float;
    props[7].data.dataUnion.Float = m_camera.MovementSpeed;

    props[8].name = "MouseSensitivity";
    props[8].type = SerializablePropertyType::Float;
    props[8].data.dataUnion.Float = m_camera.MouseSensitivity;

    props[9].name = "Zoom";
    props[9].type = SerializablePropertyType::Float;
    props[9].data.dataUnion.Float = m_camera.Zoom;

    return props;
}

bool SerializableCamera::SetSerializableProperty(const SerializableProperty& p)
{
    if (p.name == "Position") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Vec3, "Position", &m_camera.Position);
    }
    else if (p.name == "Front") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Vec3, "Front", &m_camera.Front);
    }
    else if (p.name == "Up") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Vec3, "Up", &m_camera.Up);
    }
    else if (p.name == "Right") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Vec3, "Right", &m_camera.Right);
    }
    else if (p.name == "WorldUp") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Vec3, "WorldUp", &m_camera.WorldUp);
    }
    else if (p.name == "Yaw") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Float, "Yaw", &m_camera.Yaw);
    }
    else if (p.name == "Pitch") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Float, "Pitch", &m_camera.Pitch);
    }
    else if (p.name == "MovementSpeed") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Float, "MovementSpeed", &m_camera.MovementSpeed);
    }
    else if (p.name == "MouseSensitivity") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Float, "MouseSensitivity", &m_camera.MouseSensitivity);
    }
    else if (p.name == "Zoom") {
        return ISerializable_TrySetNewPropertyVal(p, SerializablePropertyType::Float, "Zoom", &m_camera.Zoom);
    }
    else {
        return false;
    }
}

int SerializableCamera::GetNumSerializableProperties() const
{
    return NUM_CAMERA_SERIALIZABLE_PROPERTIES;
}

void SerializableCamera::SaveToFile(std::string filePath) const
{
    m_camera.SaveToFile(filePath);
}

void SerializableCamera::LoadFromFile(std::string filePath)
{
    m_camera.LoadFromFile(filePath);
}


const std::string& SerializableCamera::GetName()
{
    static const std::string cameraName = "SerializableCamera";
    return cameraName;
}
