/*
Copyright © 2017-2018,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance for Sustainable Energy, LLC
All rights reserved. See LICENSE file and DISCLAIMER for more details.
*/
#pragma once

#include "MessageFederate.hpp"
#include <mutex>
#include <type_traits>

namespace helics
{
/** class to manage an endpoint */
class Endpoint
{
  private:
    MessageFederate *fed = nullptr;  //!< the MessageFederate to interact with
    interface_handle handle;  //!< the id as generated by the Federate
    int referenceIndex = -1;  //!< an index used for callback lookup
    void *dataReference = nullptr;  //!< pointer to a piece of containing data
    std::string targetDest;  //!< a predefined target destination
    std::string actualName;  //!< the name of the federate
  public:
    /** default constructor*/
    Endpoint () = default;
    /**/
    Endpoint (MessageFederate *mfed, interface_handle id, const std::string &actName)
        : fed (mfed), handle (id), actualName (actName)
    {
    }
    /** check if the Endpoint links to a valid operation*/
    bool isValid () const { return handle.isValid (); }
    bool operator== (const Endpoint &ept) const { return (handle == ept.handle); }
    bool operator!= (const Endpoint &ept) const { return (handle != ept.handle); }
    bool operator< (const Endpoint &ept) const { return (handle < ept.handle); }
    /** send a data block and length
    @param[in] dest string name of the destination
    @param[in] data pointer to data location
    @param[in] data_size the length of the data
    */
    void send (const std::string &dest, const char *data, size_t data_size) const
    {
        fed->sendMessage (*this, dest, data_view (data, data_size));
    }

    /** subscribe the endpoint to a particular publication*/
    void subscribe (const std::string &key) { fed->subscribe (*this, key); }
    /** send a data block and length
    @param[in] dest string name of the destination
    @param[in] data pointer to data location
    @param[in] data_size the length of the data
    */
    void send (const std::string &dest, const char *data, size_t data_size, Time sendTime) const
    {
        fed->sendMessage (*this, dest, data_view (data, data_size), sendTime);
    }
    /** send a data block and length
    @param[in] data pointer to data location
    @param[in] data_size the length of the data
    @param sendTime the time to send the message
    */
    void send (const char *data, size_t data_size, Time sendTime) const
    {
        fed->sendMessage (*this, targetDest, data_view (data, data_size), sendTime);
    }
    /** send a data_view
    @details a data view can convert from many different formats so this function should
    be catching many of the common use cases
    @param[in] dest string name of the destination
    @param[in] data the information to send
    */
    void send (const std::string &dest, data_view data) const { fed->sendMessage (*this, dest, data); }
    /** send a data_view
    @details a data view can convert from many different formats so this function should
    be catching many of the common use cases
    @param[in] dest string name of the destination
    @param[in] data data representation to send
    @param[in] sendTime  the time the message should be sent
    */
    void send (const std::string &dest, data_view data, Time sendTime) const
    {
        fed->sendMessage (*this, dest, data, sendTime);
    }
    /** send a data block and length to the target destination
    @param[in] data pointer to data location
    @param[in] data_size the length of the data
    */
    void send (const char *data, size_t data_size) const
    {
        fed->sendMessage (*this, targetDest, data_view (data, data_size));
    }
    /** send a data_view to the target destination
    @details a data view can convert from many different formats so this function should
    be catching many of the common use cases
    @param[in] data the information to send
    */
    void send (data_view data) const { fed->sendMessage (*this, targetDest, data); }
    /** send a data_view to the specified target destination
    @details a data view can convert from many different formats so this function should
    be catching many of the common use cases
    @param[in] data a representation to send
    @param[in] sendTime  the time the message should be sent
    */
    void send (data_view data, Time sendTime) const { fed->sendMessage (*this, targetDest, data, sendTime); }
    /** send a message object
    @details this is to send a pre-built message
    @param[in] mess a reference to an actual message object
    */
    void send (const Message &mess) const { fed->sendMessage (*this, mess); }
    /** get an available message if there is no message the returned object is empty*/
    auto getMessage () const { return fed->getMessage (*this); }
    /** check if there is a message available*/
    bool hasMessage () const { return fed->hasMessage (*this); }
    /** check if there is a message available*/
    auto pendingMessages () const { return fed->pendingMessages (*this); }
    /** register a callback for an update notification
    @details the callback is called in the just before the time request function returns
    @param[in] callback a function with signature void(endpoint_id_t, Time)
    time is the time the value was updated  This callback is a notification callback and doesn't return the value
    */
    void setCallback (const std::function<void(const Endpoint &, Time)> &callback)
    {
        fed->registerEndpointCallback (*this, callback);
    }

    /** add a named filter to an endpoint for all message coming from the endpoint*/
    void addSourceFilter (const std::string &filterName) { fed->addSourceFilter (*this, filterName); }
    /** add a named filter to an endpoint for all message going to the endpoint*/
    void addDestinationFilter (const std::string &filterName) { fed->addDestinationFilter (*this, filterName); }
    /** set a target destination for unspecified messages*/
    void setTargetDestination (const std::string &target) { targetDest = target; }
    /** get the name of the endpoint*/
    const std::string &getName () const { return actualName; }
    /** get the specified type of the endpoint*/
    const std::string &getType () const { return fed->getEndpointType (*this); }
    /** get the actual endpoint id for the fed*/
    interface_handle getID () const { return handle; }

    friend class MessageFederateManager;
};
}  // namespace helics
