#define pSubTo(s,a,b) \
	((a).smaller((b))?(s)=~(s),(b).To_diff((a),0):(a).Diff((b),0))

#define SigAddTo(s,a,_,b) \
	((s)^(_)?(pSubTo((s),(a),(b))):((a).Add((b))))

#define SigSubTo(s,a,_,b) \
	((s)^(_)?((a).Add((b))):(pSubTo((s),(a),(b))))
