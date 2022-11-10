#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>

#define DF_OFFSET 0x4000
#define NO_DF_OFFSET 0x3FFF

#define IP_STR(addr) ((unsigned char *)&addr)[0], ((unsigned char *)&addr)[1], ((unsigned char *)&addr)[2], ((unsigned char *)&addr)[3]

static struct nf_hook_ops nfho;
struct iphdr *iph;

unsigned int hook_func(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
	// hook_func
	if (!skb) { return NF_ACCEPT; }

	iph = (struct iphdr *)ip_hdr(skb);
	if (!iph) { return NF_ACCEPT; }

	if (ntohs(iph->frag_off) & DF_OFFSET) {
		//printk(KERN_INFO "PF-MOD-NO-DF: Flags: [%u] Len: [%u] src: [%d.%d.%d.%d] dst: [%d.%d.%d.%d]\n", ntohs(iph->frag_off), ntohs(iph->tot_len), IP_STR(iph->saddr), IP_STR(iph->daddr));
		iph->frag_off = (iph->frag_off & htons(NO_DF_OFFSET));
		iph->check = 0;
		iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
	}

	return NF_ACCEPT;
}

static int __init initialize(void) {
	struct net *n;
	nfho.hook = hook_func;
	nfho.hooknum = NF_INET_PRE_ROUTING;
	nfho.pf = NFPROTO_IPV4;
	nfho.priority = NF_IP_PRI_FIRST;

	for_each_net(n)
		nf_register_net_hook(n, &nfho);

	return 0;
}

static void __exit teardown(void) {
	struct net *n;

	for_each_net(n)
		nf_unregister_net_hook(n, &nfho);

}

module_init(initialize);
module_exit(teardown);
MODULE_LICENSE("GPL");
