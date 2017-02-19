//
// Creating a Vulkan instance
//

#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>

using namespace std;

VkInstance inst;

void initInstance() {
    // Application info
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = "instance";
    app_info.applicationVersion = 1;
    app_info.pEngineName = "Huevos";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    // Vulkan instance
    VkInstanceCreateInfo inst_info = {};
    inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    inst_info.pNext = NULL;
    inst_info.flags = 0;
    inst_info.pApplicationInfo = &app_info;
    inst_info.enabledExtensionCount = 0;
    inst_info.ppEnabledExtensionNames = NULL;
    inst_info.enabledLayerCount = 0;
    inst_info.ppEnabledLayerNames = NULL;

    VkResult res;

    res = vkCreateInstance(&inst_info, NULL, &inst);

    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        cout << "Could not find a compatible Vulkan ICD" << endl;
        exit(-1);
    } else if (res) {
        cout << "Unknown error: " << res << endl;
        exit(-1);
    }
}

int main() {
    initInstance();

    cout << "Vulkan instance created successfully" << endl;

    VkResult res;
    uint32_t dev_count = 0;
    res = vkEnumeratePhysicalDevices(inst, &dev_count, NULL);
    if (res) {
        cout << "could not determine the number of devices" << endl;
        vkDestroyInstance(inst, NULL);
        exit(-1);
    }

    cout << "Found " << dev_count << " devices" << endl;

    if (dev_count == 0) {
        cout << "No compatible devices found, terminating" << endl;
        exit(-2);
    }

    vector<VkPhysicalDevice> devices;
    devices.resize(dev_count);
    res = vkEnumeratePhysicalDevices(inst, &dev_count, devices.data());
    if (res) {
        cout << "error enumerating devices" << endl;
        vkDestroyInstance(inst, NULL);
        exit(-1);
    }

    // seleciona o primeiro dispositivo encontrado
    VkPhysicalDevice& dev = devices[0];

    uint32_t nFamilies;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &nFamilies, nullptr);
    cout << "  Found " << nFamilies << " queue families for selected device" << endl;
    vector<VkQueueFamilyProperties> queueProps;
    queueProps.resize(nFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &nFamilies, queueProps.data());

    // procura uma familia de filas com capacidade para graficos
    VkDeviceQueueCreateInfo queueInfo;

    bool found = false;
    for (unsigned int i = 0; i < nFamilies; ++i) {
        if (queueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            found = true;
            queueInfo.queueFamilyIndex = i;
            break;
        }
    }

    if (not found) {
        cout << "Could not find a graphics-capable queue family" << endl;
        vkDestroyInstance(inst, nullptr);
        exit(-1);
    }

    float priorities[1] = { 0.0f };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.pNext = nullptr;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = priorities;

    VkDeviceCreateInfo devInfo = {};
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.pNext = nullptr;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &queueInfo;
    devInfo.enabledExtensionCount = 0;
    devInfo.ppEnabledExtensionNames = nullptr;
    devInfo.enabledLayerCount = 0;
    devInfo.ppEnabledLayerNames = nullptr;
    devInfo.pEnabledFeatures = nullptr;

    VkDevice device;
    res = vkCreateDevice(dev, &devInfo, nullptr, &device);
    if (res) {
        cout << "Error creating logical device" << endl;
        vkDestroyInstance(inst, nullptr);
        exit(-1);
    }

    cout << "Device created successfully" << endl;

    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(inst, nullptr);

    return 0;
}

