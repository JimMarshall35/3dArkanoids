#pragma once
#include "ISerializable.h"
#include "Camera.h"
#include <string>
class SerializableCamera :
    public ISerializable
{
public:
    SerializableCamera(std::string path);
    // Inherited via ISerializable
    virtual const std::vector<SerializableProperty>& GetSerializableProperties() override;
    virtual bool SetSerializableProperty(const SerializableProperty& p) override;
    virtual int GetNumSerializableProperties() const override;
    virtual void SaveToFile(std::string filePath) const override;
    virtual void LoadFromFile(std::string filePath) override;
    inline Camera& GetCamera() {
        return m_camera;
    }
private:
    Camera m_camera;

    // Inherited via ISerializable
    virtual const std::string& GetName() override;
};

