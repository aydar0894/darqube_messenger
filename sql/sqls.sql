-- TX Out
select pubkey_id, count(txout_id) as num_transactions, sum(txout_value) as out_sum
from (select txout.pubkey_id as pubkey_id, txout.txout_value as txout_value, txout.txout_id as txout_id
      from pubkey inner join txout on pubkey.pubkey_id=txout.pubkey_id)
      as jg
group by pubkey_id
limit 100

-- ///

-- TX In

select pubkey_id, count(txout_id) as num_transactions, sum(txout_value) as in_sum
from (select txout.pubkey_id as pubkey_id, txout.txout_value as txout_value, txout.txout_id as txout_id
      from pubkey inner join txout on pubkey.pubkey_id=txout.pubkey_id
      left join txin on txout.pubkey_id=txin.txout_id)
      as jg
group by pubkey_id
limit 100

-- ///


-- TX In for certain pubkey

select in_sum from(
select pubkey_id, count(txout_id) as num_transactions, sum(txout_value) as in_sum
from (select txout.pubkey_id as pubkey_id, txout.txout_value as txout_value, txout.txout_id as txout_id
      from pubkey inner join txout on pubkey.pubkey_id=txout.pubkey_id
      left join txin on txout.pubkey_id=txin.txout_id)
      as jg
group by pubkey_id
) as jq
where pubkey_id=124853

-- ///


-- Balances

select (in_sum - out_sum) as balance, in_t.pubkey_id as pubkey_id from(
    (select pubkey_id, count(txout_id) as num_transactions, sum(txout_value) as in_sum
    from (select txout.pubkey_id as pubkey_id, txout.txout_value as txout_value, txout.txout_id as txout_id
      from pubkey inner join txout on pubkey.pubkey_id=txout.pubkey_id
      left join txin on txout.pubkey_id=txin.txout_id)
      as ji
	group by pubkey_id) as in_t
    left join
    (select pubkey_id, count(txout_id) as num_transactions, sum(txout_value) as out_sum
    from (select txout.pubkey_id as pubkey_id, txout.txout_value as txout_value, txout.txout_id as txout_id
          from pubkey inner join txout on pubkey.pubkey_id=txout.pubkey_id)
          as jo
    group by pubkey_id) as out_t
    on out_t.pubkey_id=in_t.pubkey_id
)

-- ///
