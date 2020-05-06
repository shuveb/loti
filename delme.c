/* Supported Ops */
/*****************/

/* DONE */
io_uring_get_probe_ring(struct io_uring *ring);
/* DONE */
io_uring_get_probe(void);
/* DONE */
io_uring_opcode_supported(struct io_uring_probe *p, int op)

/* Setup & tear down */
/*********************/

/* DONE */
extern int io_uring_queue_init(unsigned entries, struct io_uring *ring,
	unsigned flags);
/* DONE */
extern int io_uring_queue_init_params(unsigned entries, struct io_uring *ring,
	struct io_uring_params *p);
/* DONE */
extern int io_uring_queue_mmap(int fd, struct io_uring_params *p,
	struct io_uring *ring);
/* DONE */
extern int io_uring_ring_dontfork(struct io_uring *ring);
/* DONE */
extern void io_uring_queue_exit(struct io_uring *ring);

/*Completion-related*/
/********************/
/* DONE */
static inline void *io_uring_cqe_get_data(const struct io_uring_cqe *cqe)
/* DONE */
unsigned io_uring_peek_batch_cqe(struct io_uring *ring, struct io_uring_cqe **cqes, unsigned count);
/* DONE */
extern int io_uring_wait_cqes(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, unsigned wait_nr, struct __kernel_timespec *ts, sigset_t *sigmask);
/* DONE */
extern int io_uring_wait_cqe_timeout(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, struct __kernel_timespec *ts);

/*
 * Return an IO completion, waiting for 'wait_nr' completions if one isn't
 * readily available. Returns 0 with cqe_ptr filled in on success, -errno on
 * failure.
 */
/* DONE */
static inline int io_uring_wait_cqe_nr(struct io_uring *ring, struct io_uring_cqe **cqe_ptr, unsigned wait_nr)

/*
 * Return an IO completion, if one is readily available. Returns 0 with
 * cqe_ptr filled in on success, -errno on failure.
 */
/* DONE */
static inline int io_uring_peek_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr)
/*
 * Return an IO completion, waiting for it if necessary. Returns 0 with
 * cqe_ptr filled in on success, -errno on failure.
 */
/* DONE */
static inline int io_uring_wait_cqe(struct io_uring *ring, struct io_uring_cqe **cqe_ptr)

/* Completion-related helpers */
/******************************/
io_uring_for_each_cqe(ring, head, cqe)				
/* DONE */
static inline void io_uring_cq_advance(struct io_uring *ring, unsigned nr)
/*
 * Must be called after io_uring_{peek,wait}_cqe() after the cqe has
 * been processed by the application.
 */
/* DONE */
static inline void io_uring_cqe_seen(struct io_uring *ring, struct io_uring_cqe *cqe)

static inline unsigned io_uring_cq_ready(struct io_uring *ring)
	
/* Submission-related */
/**********************/
/* DONE */
extern int io_uring_submit(struct io_uring *ring);
/* DONE */
extern int io_uring_submit_and_wait(struct io_uring *ring, unsigned wait_nr);
/* DONE */
extern struct io_uring_sqe *io_uring_get_sqe(struct io_uring *ring);
/* DONE */
static inline void io_uring_sqe_set_data(struct io_uring_sqe *sqe, void *data)
/* DONE */
static inline void io_uring_sqe_set_flags(struct io_uring_sqe *sqe,
					  unsigned flags)

/*
 * submission helpers
 */
/* DONE */
static inline void io_uring_prep_splice(struct io_uring_sqe *sqe,
					int fd_in, loff_t off_in,
					int fd_out, loff_t off_out,
					unsigned int nbytes,
					unsigned int splice_flags)
/* DONE */
static inline void io_uring_prep_readv(struct io_uring_sqe *sqe, int fd,
				       const struct iovec *iovecs,
				       unsigned nr_vecs, off_t offset)
/* DONE */
static inline void io_uring_prep_read_fixed(struct io_uring_sqe *sqe, int fd,
					    void *buf, unsigned nbytes,
					    off_t offset, int buf_index)
/* DONE */
static inline void io_uring_prep_writev(struct io_uring_sqe *sqe, int fd,
					const struct iovec *iovecs,
					unsigned nr_vecs, off_t offset)
/* DONE */
static inline void io_uring_prep_write_fixed(struct io_uring_sqe *sqe, int fd,
					     const void *buf, unsigned nbytes,
					     off_t offset, int buf_index)
/* DONE */
static inline void io_uring_prep_recvmsg(struct io_uring_sqe *sqe, int fd,
					 struct msghdr *msg, unsigned flags)
/* DONE */
static inline void io_uring_prep_sendmsg(struct io_uring_sqe *sqe, int fd,
					 const struct msghdr *msg, unsigned flags)
/* DONE */
static inline void io_uring_prep_poll_add(struct io_uring_sqe *sqe, int fd,
					  short poll_mask)
/* DONE */
static inline void io_uring_prep_poll_remove(struct io_uring_sqe *sqe,
					     void *user_data)
/* DONE */
static inline void io_uring_prep_fsync(struct io_uring_sqe *sqe, int fd,
				       unsigned fsync_flags)
/* DONE */
static inline void io_uring_prep_nop(struct io_uring_sqe *sqe)
/* DONE */
static inline void io_uring_prep_accept(struct io_uring_sqe *sqe, int fd,
					struct sockaddr *addr,
					socklen_t *addrlen, int flags)
/* DONE */
static inline void io_uring_prep_connect(struct io_uring_sqe *sqe, int fd,
					 struct sockaddr *addr,
					 socklen_t addrlen)
/* DONE */
static inline void io_uring_prep_fallocate(struct io_uring_sqe *sqe, int fd,
					   int mode, off_t offset, off_t len)
/* DONE */
static inline void io_uring_prep_openat(struct io_uring_sqe *sqe, int dfd,
					const char *path, int flags, mode_t mode)
/* DONE */
static inline void io_uring_prep_close(struct io_uring_sqe *sqe, int fd)
/* DONE */
static inline void io_uring_prep_read(struct io_uring_sqe *sqe, int fd,
				      void *buf, unsigned nbytes, off_t offset)
/* DONE */
static inline void io_uring_prep_write(struct io_uring_sqe *sqe, int fd,
				       const void *buf, unsigned nbytes, off_t offset)
/* DONE */
static inline void io_uring_prep_statx(struct io_uring_sqe *sqe, int dfd,
				const char *path, int flags, unsigned mask,
				struct statx *statxbuf)
/* DONE */
static inline void io_uring_prep_fadvise(struct io_uring_sqe *sqe, int fd,
					 off_t offset, off_t len, int advice)
/* DONE */
static inline void io_uring_prep_madvise(struct io_uring_sqe *sqe, void *addr,
					 off_t length, int advice)
/* DONE */
static inline void io_uring_prep_send(struct io_uring_sqe *sqe, int sockfd,
				      const void *buf, size_t len, int flags)
/* DONE */
static inline void io_uring_prep_recv(struct io_uring_sqe *sqe, int sockfd,
				      void *buf, size_t len, int flags)
/* DONE */
static inline void io_uring_prep_openat2(struct io_uring_sqe *sqe, int dfd,
					const char *path, struct open_how *how)
/* DONE */
static inline void io_uring_prep_epoll_ctl(struct io_uring_sqe *sqe, int epfd,
					   int fd, int op,
					   struct epoll_event *ev)

/* Advanced use */
/* DONE */
extern int io_uring_register_buffers(struct io_uring *ring,
					const struct iovec *iovecs,
					unsigned nr_iovecs);
/* DONE */
extern int io_uring_unregister_buffers(struct io_uring *ring);
/* DONE */
extern int io_uring_register_files(struct io_uring *ring, const int *files,
					unsigned nr_files);
/* DONE */
extern int io_uring_unregister_files(struct io_uring *ring);
/* DONE */
extern int io_uring_register_files_update(struct io_uring *ring, unsigned off,
					int *files, unsigned nr_files);
/* DONE */
extern int io_uring_register_eventfd(struct io_uring *ring, int fd);
/* DONE */
extern int io_uring_register_eventfd_async(struct io_uring *ring, int fd);
/* DONE */
extern int io_uring_unregister_eventfd(struct io_uring *ring);
/* DONE */
extern int io_uring_register_probe(struct io_uring *ring,
					struct io_uring_probe *p, unsigned nr);
/* DONE */
extern int io_uring_register_personality(struct io_uring *ring);
/* DONE */
extern int io_uring_unregister_personality(struct io_uring *ring, int id);

static inline void io_uring_prep_files_update(struct io_uring_sqe *sqe,
					      int *fds, unsigned nr_fds,
					      int offset)
static inline void io_uring_prep_provide_buffers(struct io_uring_sqe *sqe,
						 void *addr, int len, int nr,
						 int bgid, int bid)
static inline void io_uring_prep_remove_buffers(struct io_uring_sqe *sqe,
						int nr, int bgid)
static inline unsigned io_uring_sq_ready(struct io_uring *ring)
static inline unsigned io_uring_sq_space_left(struct io_uring *ring)


static inline void io_uring_prep_timeout(struct io_uring_sqe *sqe,
					 struct __kernel_timespec *ts,
					 unsigned count, unsigned flags)
static inline void io_uring_prep_timeout_remove(struct io_uring_sqe *sqe,
						__u64 user_data, unsigned flags)
static inline void io_uring_prep_cancel(struct io_uring_sqe *sqe, void *user_data,
					int flags)
static inline void io_uring_prep_link_timeout(struct io_uring_sqe *sqe,
					      struct __kernel_timespec *ts,
					      unsigned flags)
