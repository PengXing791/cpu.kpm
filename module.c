#include <kpm.h>
#include <fs.h>
#include <linux/dcache.h>

KPM_MODULE("cpuspoof");

static const char fake_cpuinfo[] =
"Processor       : AArch64 Processor rev 0 (aarch64)\n"
"Features        : fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x1\n"
"CPU part        : 0xd0c\n"
"CPU revision    : 0\n\n"
"processor       : 0\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x1\n"
"CPU part        : 0xd0c\n"
"CPU revision    : 0\n\n"
"processor       : 1\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x2\n"
"CPU part        : 0xd0a\n"
"CPU revision    : 0\n\n"
"processor       : 2\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x2\n"
"CPU part        : 0xd0a\n"
"CPU revision    : 0\n\n"
"processor       : 3\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x2\n"
"CPU part        : 0xd0a\n"
"CPU revision    : 0\n\n"
"processor       : 4\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x3\n"
"CPU part        : 0xd0b\n"
"CPU revision    : 0\n\n"
"processor       : 5\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x3\n"
"CPU part        : 0xd0b\n"
"CPU revision    : 0\n\n"
"processor       : 6\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x3\n"
"CPU part        : 0xd0b\n"
"CPU revision    : 0\n\n"
"processor       : 7\n"
"BogoMIPS        : 26.00\n"
"CPU implementer : 0x48\n"
"CPU architecture: 8\n"
"CPU variant     : 0x3\n"
"CPU part        : 0xd0b\n"
"CPU revision    : 0\n\n"
"Hardware        : Kirin9000S\n";

static ssize_t (*orig_proc_file_read)(struct file *file, char __user *buf, size_t count, loff_t *ppos);

static ssize_t hook_proc_file_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    const struct dentry *dent = file->f_path.dentry;
    if (dent && !strcmp(dent->d_name.name, "cpuinfo"))
    {
        size_t len = strlen(fake_cpuinfo);
        if (*ppos >= len)
            return 0;
        if (count > len - *ppos)
            count = len - *ppos;
        copy_to_user(buf, fake_cpuinfo + *ppos, count);
        *ppos += count;
        return count;
    }
    return orig_proc_file_read(file, buf, count, ppos);
}

int kpm_main(void)
{
    KPM_HOOK(proc_file_read, orig_proc_file_read, hook_proc_file_read);
    return 0;
}
