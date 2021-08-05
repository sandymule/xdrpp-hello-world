#include "signature_check_api_server.h"

namespace edce {

SignatureCheckApiServer::SignatureCheckApiServer()
    : ps()
    , signature_check_listener(ps, xdr::tcp_listen(SIGNATURE_CHECK_PORT, AF_INET), false, xdr::session_allocator<void>()) {
        signature_check_listener.register_service(signature_check_server);
        init_ping_shard();
        ps.run();
        //std::thread th([this] {ps.run();});
        //th.detach();
    }

void
SignatureCheckApiServer::init_ping_shard()
{
    int idx = 1;
    std::string ip_addr = std::string("10.10.1.") + std::to_string(idx);
    auto fd = xdr::tcp::connect(ip_addr.c_str(), SIGNATURE_CHECK_PORT);
    auto client = xdr::srpc_client<SignatureCheckerConnectV1>(fd.get());

    //client.hello_world();

    std::cout << "stuff" << std::endl;
}


/*

void
ConsensusApiServer::add_to_pollset(xdr::pollset& ps) {

    BLOCK_INFO("running consensus_api_server");

    bt_listener = xdr::arpc_tcp_listener<> (ps, xdr::tcp_listen(BLOCK_FORWARDING_PORT, AF_INET), false, xdr::session_allocator<void>());

    ack_listener = xdr::arpc_tcp_listener<> (ps, xdr::tcp_listen(BLOCK_CONFIRMATION_PORT, AF_INET), false, xdr::session_allocator<void>());

    req_listener = xdr::srpc_tcp_listener<> (ps, xdr::tcp_listen(FORWARDING_REQUEST_PORT, AF_INET), false, xdr::session_allocator<void>());

    control_listener = xdr::srpc_tcp_listener<> (ps, xdr::tcp_listen(SERVER_CONTROL_PORT, AF_INET), false, xdr::session_allocator<void>());
    
    //ps.run();
    //throw std::runtime_error("pollset.run() should never terminate!");
}*/

} /* edce */