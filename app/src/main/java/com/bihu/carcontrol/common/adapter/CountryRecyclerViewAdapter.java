package com.bihu.carcontrol.common.adapter;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.bihu.carcontrol.R;
import com.bihu.carcontrol.common.bean.CountryBean;

import java.util.List;

/**
 * FileName: CountryRecyclerViewAdapter
 * Author: WangXu
 * Date: 2024/12/20 13:56
 * Description:
 */
public class CountryRecyclerViewAdapter extends RecyclerView.Adapter<CountryRecyclerViewAdapter.CountryViewHolder> {
    private List<CountryBean> countryBeans;
    private int selectCountryCode;
    private Context context;
    private onItemClickListener onItemClickListener;

    public interface onItemClickListener {
        void onCountryItemClick(CountryBean countryBean);
    }

    public void setOnItemClick(onItemClickListener onItemClickListener) {
        this.onItemClickListener = onItemClickListener;
    }

    public CountryRecyclerViewAdapter(Context context, List<CountryBean> countryBeans, int selectCountryCode) {
        this.countryBeans = countryBeans;
        this.selectCountryCode = selectCountryCode;
        this.context = context;
    }

    @NonNull
    @Override
    public CountryRecyclerViewAdapter.CountryViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int i) {
        View view = LayoutInflater.from(context).inflate(R.layout.item_country_select, viewGroup, false);
        return new CountryViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull CountryViewHolder countryViewHolder, int i) {
        CountryBean countryBean = countryBeans.get(i);
        if (selectCountryCode == countryBean.getCountryCode()) {
            countryViewHolder.ivSelectCountry.setVisibility(View.VISIBLE);
        } else {
            countryViewHolder.ivSelectCountry.setVisibility(View.INVISIBLE);
        }
        countryViewHolder.tvCountryName.setText(countryBean.getCountryName());
        countryViewHolder.itemView.setOnClickListener(v -> {
            onItemClickListener.onCountryItemClick(countryBean);
        });
    }

    @Override
    public int getItemCount() {
        return countryBeans.size();
    }

    public static class CountryViewHolder extends RecyclerView.ViewHolder {
        private ImageView ivSelectCountry;
        private TextView tvCountryName;

        public CountryViewHolder(@NonNull View itemView) {
            super(itemView);
            ivSelectCountry = itemView.findViewById(R.id.iv_select_country);
            tvCountryName = itemView.findViewById(R.id.tv_country_name);
        }
    }
}
