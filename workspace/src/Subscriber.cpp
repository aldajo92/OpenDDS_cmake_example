#include "ExampleTypeSupportImpl.h"
#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>
#include <iostream>
#include <thread>

class ExampleDataReaderListener : public DDS::DataReaderListener
{
public:
    // Callback for when data is available
    virtual void on_data_available(DDS::DataReader_ptr reader) override
    {
        ExampleModule::ExampleMessageDataReader_var reader_i = ExampleModule::ExampleMessageDataReader::_narrow(reader);
        if (!reader_i)
        {
            std::cerr << "Failed to narrow DataReader!" << std::endl;
            return;
        }

        ExampleModule::ExampleMessageSeq messages;
        DDS::SampleInfoSeq info_seq;
        DDS::ReturnCode_t error = reader_i->take(
            messages, info_seq, DDS::LENGTH_UNLIMITED,
            DDS::ANY_SAMPLE_STATE, DDS::ANY_VIEW_STATE, DDS::ANY_INSTANCE_STATE);

        if (error == DDS::RETCODE_OK)
        {
            for (CORBA::ULong i = 0; i < messages.length(); ++i)
            {
                if (info_seq[i].valid_data)
                {
                    std::cout << "Received message: ID = " << messages[i].id
                              << ", Text = " << messages[i].text << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "Failed to take data: " << error << std::endl;
        }
    }

    // Empty implementations for other callbacks
    virtual void on_requested_deadline_missed(DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus &) override {}
    virtual void on_requested_incompatible_qos(DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus &) override {}
    virtual void on_sample_rejected(DDS::DataReader_ptr, const DDS::SampleRejectedStatus &) override {}
    virtual void on_liveliness_changed(DDS::DataReader_ptr, const DDS::LivelinessChangedStatus &) override {}
    virtual void on_subscription_matched(DDS::DataReader_ptr, const DDS::SubscriptionMatchedStatus &) override {}
    virtual void on_sample_lost(DDS::DataReader_ptr, const DDS::SampleLostStatus &) override {}
};
void run_subscriber()
{
    int argc = 0;
    DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, nullptr);

    DDS::DomainParticipant_var participant = dpf->create_participant(
        42, PARTICIPANT_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!participant)
    {
        std::cerr << "Failed to create DomainParticipant!" << std::endl;
        return;
    }

    ExampleModule::ExampleMessageTypeSupportImpl *ts = new ExampleModule::ExampleMessageTypeSupportImpl;
    if (ts->register_type(participant, "") != DDS::RETCODE_OK)
    {
        std::cerr << "Failed to register TypeSupport!" << std::endl;
        return;
    }

    CORBA::String_var type_name = ts->get_type_name();
    DDS::Topic_var topic = participant->create_topic(
        "ExampleTopic", type_name, TOPIC_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!topic)
    {
        std::cerr << "Failed to create Topic!" << std::endl;
        return;
    }

    DDS::Subscriber_var subscriber = participant->create_subscriber(
        SUBSCRIBER_QOS_DEFAULT, nullptr, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!subscriber)
    {
        std::cerr << "Failed to create Subscriber!" << std::endl;
        return;
    }

    // Create DataReader with the custom listener
    DDS::DataReader_var reader = subscriber->create_datareader(
        topic, DATAREADER_QOS_DEFAULT, new ExampleDataReaderListener, OpenDDS::DCPS::DEFAULT_STATUS_MASK);

    if (!reader)
    {
        std::cerr << "Failed to create DataReader!" << std::endl;
        return;
    }

    std::cout << "Waiting for messages..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(60));

    participant->delete_contained_entities();
    dpf->delete_participant(participant);
}
