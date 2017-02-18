//
// Enumerating and getting info from devices
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

const char* deviceType(VkPhysicalDeviceType type) {
    switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "CPU";

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "GPU (integrated)";

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "GPU (discrete)";

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "GPU (virtual)";

        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "Other";

        default:
            return "Unknown";
    }
}

const char* vendorIDStr(uint32_t id) {
    switch (id) {
        case 0x1002:
            return "AMD";

        case 0x10DE:
            return "NVidia";

        case 0x8086:
            return "Intel";

        default:
            return "Other";
    }
}

void printDeviceProperties(VkPhysicalDevice& dev) {
    VkPhysicalDeviceProperties prop;

    vkGetPhysicalDeviceProperties(dev, &prop);
    cout << "Name: " << prop.deviceName << endl;
    cout << "Type: " << deviceType(prop.deviceType) << endl;
    cout << "Vendor ID: " << hex << prop.vendorID << " (" << vendorIDStr(prop.vendorID) << ")" << endl;
    cout << "Device ID: " << hex << prop.deviceID << endl;
    cout << endl;
}

void printQueueFlags(VkQueueFlags flags) {
    if (flags & VK_QUEUE_COMPUTE_BIT)
        cout << "compute ";
    if (flags & VK_QUEUE_GRAPHICS_BIT)
        cout << "graphics ";
    if (flags & VK_QUEUE_TRANSFER_BIT)
        cout << "blit ";
    if (flags & VK_QUEUE_SPARSE_BINDING_BIT)
        cout << "sparsebind";
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

    cout << "Found " << dev_count << " devices" << endl << endl;
    vector<VkPhysicalDevice> devices;
    devices.resize(dev_count);
    res = vkEnumeratePhysicalDevices(inst, &dev_count, devices.data());
    if (res) {
        cout << "error enumerating devices" << endl;
        vkDestroyInstance(inst, NULL);
        exit(-1);
    }

    for (int i = 0; i < dev_count; ++i) {
        cout << "=== Device #" << i << endl;
        printDeviceProperties(devices[i]);

        // queues
        uint32_t nFamilies;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &nFamilies, nullptr);
        cout << "  Found " << nFamilies << " queue families for device " << i << endl;
        vector<VkQueueFamilyProperties> queueProps;
        queueProps.resize(nFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &nFamilies, queueProps.data());
        for (int j = 0; j < nFamilies; ++j) {
            cout << "  --- Queue family #" << j << endl;
            cout << "  Queue count: " << queueProps[j].queueCount << endl;
            cout << "  Flags: ";
            printQueueFlags(queueProps[j].queueFlags);
            cout << endl;
        }
    }


    //VkQueueFamilyProperties
    vkDestroyInstance(inst, NULL);

    return 0;
}


