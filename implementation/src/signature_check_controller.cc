#include "xdr/experiments.h"
#include "xdr/signature_check_api.h"
#include "rpc/rpcconfig.h"
#include <xdrpp/srpc.h>
#include <thread>
#include <chrono>

#include "utils.h"

#include <cstdint>
#include <vector>

#include "crypto_utils.h"
#include "utils.h"
#include "xdr/experiments.h"
#include "edce_management_structures.h"
#include "tbb/global_control.h"


using namespace edce;

std::string hostname_from_idx(int idx) {
    return std::string("10.10.1.") + std::to_string(idx);
}

void
poll_node(int idx) {
    
    auto fd = xdr::tcp_connect(hostname_from_idx(idx).c_str(), SIGNATURE_CHECK_PORT);
    auto client = xdr::srpc_client<SignatureCheckV1>(fd.get());

    uint32_t return_value = *client.check_all_signatures();
    std::cout << return_value << std::endl;
}


int main(int argc, char const *argv[]) {

    if (argc != 4) {
        std::printf("usage: ./signature_check_controller experiment_name block_number num_threads\n");
        return -1;
    }

    DeterministicKeyGenerator key_gen;

    ExperimentParameters params;

    std::string experiment_root = std::string("experiment_data/") + std::string(argv[1]);

    std::string params_filename = experiment_root + std::string("/params");

    if (load_xdr_from_file(params, params_filename.c_str())) {
        throw std::runtime_error("failed to load params file");
    }

    EdceManagementStructures management_structures(
        20,
        ApproximationParameters {
            .tax_rate = 10,
            .smooth_mult = 10
        });

    std::printf("num accounts: %u\n", params.num_accounts);

    AccountIDList account_id_list;

    auto accounts_filename = experiment_root + std::string("/accounts");
    if (load_xdr_from_file(account_id_list, accounts_filename.c_str())) {
        throw std::runtime_error("failed to load accounts list " + accounts_filename);
    }

    std::vector<PublicKey> pks;
    pks.resize(account_id_list.size());
    tbb::parallel_for(
        tbb::blocked_range<size_t>(0, account_id_list.size()),
        [&key_gen, &account_id_list, &pks](auto r) {
            for (size_t i = r.begin(); i < r.end(); i++) {
                auto [_, pk] = key_gen.deterministic_key_gen(account_id_list[i]);
                pks[i] = pk;
            }
        });


    poll_node(2);

    return 0;

}



















