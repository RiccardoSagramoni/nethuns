#include <nethuns/nethuns.h>
#include <stdio.h>

void dump_packet(nethuns_pkthdr_t const *hdr, const unsigned char *frame)
{
    int i = 0;

    printf("%u:%u snap:%u len:%u offload{tci:%x tpid:%x} packet{tci:%x pid:%x} => [tci:%x tpid:%x vid:%d] rxhash:0x%x| ", nethuns_tstamp_sec(hdr)
                                                                                     , nethuns_tstamp_nsec(hdr)
                                                                                     , nethuns_snaplen(hdr)
                                                                                     , nethuns_len(hdr)
                                                                                     , nethuns_offvlan_tci(hdr)
                                                                                     , nethuns_offvlan_tpid(hdr)
                                                                                     , nethuns_pktvlan_tci(frame)
                                                                                     , nethuns_pktvlan_tpid(frame)
                                                                                     , nethuns_vlan_tci(hdr, frame)
                                                                                     , nethuns_vlan_tpid(hdr, frame)
                                                                                     , nethuns_vlan_vid(nethuns_vlan_tci(hdr, frame))
                                                                                     , nethuns_rxhash(hdr));
    for(; i < 14; i++)
    {
        printf("%02x ", frame[i]);
    }
    printf("\n");
}


int
main(int argc, char *argv[])
{
    nethuns_socket_t *s;

    if (argc < 2)
    {
        fprintf(stderr,"usage: %s dev\n", argv[0]);
        return 0;
    }

    struct nethuns_socket_options opt =
    {
        .numblocks       = 4
    ,   .numpackets      = 65536
    ,   .packetsize      = 2048
    ,   .dir             = nethuns_in_out
    ,   .mode            = nethuns_cap_default
    ,   .promisc         = true
    ,   .rxhash          = true
    ,   .tx_qdisc_bypass = false
    };

    char errbuf[NETHUNS_ERRBUF_SIZE];

    s = nethuns_open(&opt, errbuf);
    if (!s)
    {
        fprintf(stderr, "%s\n", errbuf);
        return -1;
    }

    if (nethuns_bind(s, argv[1], NETHUNS_ANY_QUEUE) < 0)
    {
        fprintf(stderr, "%s\n", nethuns_error(s));
        return -1;
    }

    const unsigned char *frame;
    const nethuns_pkthdr_t *pkthdr;

    for(;;)
    {
        uint64_t pkt_id;

        if ((pkt_id = nethuns_recv(s, &pkthdr, &frame)))
        {
            dump_packet(pkthdr, frame);
            nethuns_release(s, pkt_id);
        }
    }

    nethuns_close(s);
    return 0;
}

