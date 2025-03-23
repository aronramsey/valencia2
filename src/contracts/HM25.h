using namespace QPI;

struct HM252 
{
};

struct HM25 : public ContractBase
{
    struct AddService_input 
    {
        Array<sint8, 64> companyName;
        Array<sint8, 64> title;
        Array<sint8, 128> imageRef;
        uint64 price;
        uint32 estimatedTime;
        Array<sint8, 256> description;
    };
    struct AddService_output 
    {
        uint64 newServiceId;
    };

    struct GetService_input 
    {
        uint64 serviceId;
    };
    struct GetService_output 
    {
        Array<sint8, 64> companyName;
        Array<sint8, 64> title;
        Array<sint8, 128> imageRef;
        uint64 price;
        uint32 estimatedTime;
        Array<sint8, 256> description;
        uint8 status;
        uint8 averageRating;
        uint32 reviewCount;
    };

protected:
    struct ServiceData 
    {
        Array<sint8, 64> companyName;
        Array<sint8, 64> title;
        Array<sint8, 128> imageRef;
        uint64 price;
        uint32 estimatedTime;
        Array<sint8, 256> description;
        uint8 status;
        uint32 totalRating;
        uint32 reviewCount;
        id owner;
    };

    Array<ServiceData, 100> services;
    Array<bit, 100> serviceExists;
    uint64 nextServiceId;

    PUBLIC_PROCEDURE(AddService)
        if (!qpi.getEntity(qpi.invocator()))
            return;

        uint64 id = state.nextServiceId;
        if (id >= 100)
            return;

        ServiceData& service = state.services[id];
        service.price = input.price;
        service.estimatedTime = input.estimatedTime;
        service.status = 0;
        service.totalRating = 0;
        service.reviewCount = 0;
        service.owner = qpi.invocator();

        service.companyName = input.companyName;
        service.title = input.title;
        service.imageRef = input.imageRef;
        service.description = input.description;

        state.serviceExists[id] = true;
        state.nextServiceId = id + 1;

        output.newServiceId = id;
    _

    PUBLIC_FUNCTION(GetService)
        uint64 id = input.serviceId;
        if (id < state.nextServiceId && state.serviceExists[id])
        {
            const ServiceData& service = state.services[id];

            output.companyName = service.companyName;
            output.title = service.title;
            output.imageRef = service.imageRef;
            output.description = service.description;

            output.price = service.price;
            output.estimatedTime = service.estimatedTime;
            output.status = service.status;
            output.averageRating = (service.reviewCount > 0)
                ? (uint8)(service.totalRating / service.reviewCount)
                : 0;
            output.reviewCount = service.reviewCount;
        }
        else
        {
            output.status = 255;
        }
    _

    REGISTER_USER_FUNCTIONS_AND_PROCEDURES
        REGISTER_USER_PROCEDURE(AddService, 1);
        REGISTER_USER_FUNCTION(GetService, 2);
    _

    INITIALIZE
        state.nextServiceId = 0;
        for (uint64 i = 0; i < 100; i++)
            state.serviceExists[i] = false;
    _
};
