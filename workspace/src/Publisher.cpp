#include "ExampleTypeSupportImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <iostream>
#include <thread>
#include <chrono>

void run_publisher()
{
    // Initialize DomainParticipantFactory
    int argc = 0;
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, nullptr);

    // Create DomainParticipant
    DDS::DomainParticipant_var participant = dpf->create_participant(
        42, PARTICIPANT_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant)
    {
        std::cerr << "Failed to create DomainParticipant!" << std::endl;
        return;
    }

    // Register TypeSupport
    ExampleModule::ExampleMessageTypeSupport_var ts = new ExampleModule::ExampleMessageTypeSupportImpl;
    if (ts->register_type(participant, "") != DDS::RETCODE_OK)
    {
        std::cerr << "Failed to register TypeSupport!" << std::endl;
        return;
    }

    // Create Topic
    CORBA::String_var type_name = ts->get_type_name();
    DDS::Topic_var topic = participant->create_topic(
        "ExampleTopic", type_name, TOPIC_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic)
    {
        std::cerr << "Failed to create Topic!" << std::endl;
        return;
    }

    // Create Publisher
    DDS::Publisher_var publisher = participant->create_publisher(
        PUBLISHER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!publisher)
    {
        std::cerr << "Failed to create Publisher!" << std::endl;
        return;
    }

    // Create DataWriter
    DDS::DataWriter_var writer = publisher->create_datawriter(
        topic, DATAWRITER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!writer)
    {
        std::cerr << "Failed to create DataWriter!" << std::endl;
        return;
    }

    ExampleModule::ExampleMessageDataWriter_var message_writer =
        ExampleModule::ExampleMessageDataWriter::_narrow(writer);

    if (!message_writer)
    {
        std::cerr << "Failed to narrow DataWriter!" << std::endl;
        return;
    }

    // Publish messages
    ExampleModule::ExampleMessage message;
    message.id = 1;
    message.text = "Hello from Publisher!";
    for (int i = 0; i < 10; ++i)
    {
        message.text = ("Message " + std::to_string(i + 1)).c_str();
        message.id = i + 1;

        DDS::ReturnCode_t error = message_writer->write(message, DDS::HANDLE_NIL);
        if (error != DDS::RETCODE_OK)
        {
            std::cerr << "Failed to write message: " << error << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    participant->delete_contained_entities();
    dpf->delete_participant(participant);
    TheServiceParticipant->shutdown();
}
