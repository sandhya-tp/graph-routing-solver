import matplotlib.pyplot as plt
import networkx as nx
from matplotlib.backends.backend_pdf import PdfPages
from sklearn.metrics import accuracy_score, classification_report


    

def generate_analysis_report(learned_paths, clf_dt, clf_rf, X_test_dt, y_test_dt, X_val_dt, y_val_dt, X_test_rf, y_test_rf, X_val_rf, y_val_rf,  edge_flows, edge_capacities, weight_dict, reward_log=None, output_pdf_path="Model_Analysis_Report.pdf"):
    with PdfPages(output_pdf_path) as pdf:
        
        
        # Page 1: Accuracy and Classification Report of Decision Tree
       
        fig, ax = plt.subplots(figsize=(8.5, 11))
        ax.axis("off")
        summary = ("This page summarizes the performance of the Decision Tree Classifier.\n"
            "It shows the accuracy on the test set and a detailed classification report\n"
            "including precision, recall, and F1-score for each node prediction class.\n"
        )

        accuracy = clf_dt.score(X_test_dt, y_test_dt)
        y_pred = clf_dt.predict(X_val_dt)
        val_accuracy = accuracy_score(y_val_dt, y_pred)
        
        accuracy = clf_dt.score(X_test_dt, y_test_dt)
        class_report = classification_report(y_test_dt, clf_dt.predict(X_test_dt), zero_division=0)

        text = f"""
        
    
        Supervised Learning Report - Decision Tree Classifier
        -----------------------------------------------------

        {summary}
        
        Test Accuracy: {accuracy:.4f}
        
        Validation Accuracy: {val_accuracy:.4f}

        Classification Report: 
{class_report}
        """
        ax.text(0.05, 0.95, text, va='top', fontsize=10, fontfamily='monospace')
        pdf.savefig(fig)
        plt.close()
        

        # Page 2: Accuracy and Classification Report of Random Forest
        fig, ax = plt.subplots(figsize=(8.5, 11))
        ax.axis("off")
        summary = ("This page summarizes the performance of the Random Forest Classifier.\n"
            "It shows the accuracy on the test set and a detailed classification report\n"
            "including precision, recall, and F1-score for each node prediction class.\n"
        )
        accuracy = clf_rf.score(X_test_rf, y_test_rf)
        y_pred = clf_rf.predict(X_val_rf)
        val_acc = accuracy_score(y_val_rf, y_pred)
        
        class_report = classification_report(y_test_rf, clf_rf.predict(X_test_rf),zero_division=0)

        text = f"""
        Supervised Learning Report - Random Forest Classifier
        -----------------------------------------------------

        {summary}

        Test Accuracy: {accuracy:.4f}

        Validation Accuracy: {val_acc:.4f}

        Classification Report:
        
        {class_report}
        """
        ax.text(0.05, 0.95, text, va='top', fontsize=10, fontfamily='monospace')
        pdf.savefig(fig)
        plt.close()

        # Page 3: Flow vs. Capacity Plot
        summary = (
            "This scatter plot shows how much flow has been assigned to each edge versus its capacity.\n"
            "Points above the y=x line (if added) indicate overused edges, which may violate capacity constraints."
        )
        flows = []
        capacities = []
        for (u, v), flow in edge_flows.items():
            capacity = edge_capacities.get((u, v), 0)
            flows.append(flow)
            capacities.append(capacity)
        fig, (ax_text, ax_plot) = plt.subplots(nrows=2, figsize=(8, 5), gridspec_kw={'height_ratios': [1, 2]})
        fig.subplots_adjust(hspace=-0.3)  # Add vertical space between rows

        ax_text.axis("off")
        ax_text.text(0, 1, summary, ha='left', va='top', fontsize=8, wrap=True, fontfamily='monospace')

        # Flow vs Capacity Plot (ax_plot)
        ax_plot.scatter(capacities, flows, alpha=0.7)
        ax_plot.set_title("Flow vs Capacity", fontsize=12)
        ax_plot.set_xlabel("Edge Capacity")
        ax_plot.set_ylabel("Flow Assigned")
        ax_plot.grid(False)

        pdf.savefig(fig)
        plt.close()
        #Page 4: Path Stats
        summary = (
            "This page contains statistics about the learned routing paths.\n"
            "The first histogram shows the distribution of path lengths (number of hops).\n"
            "The second shows the total transit time per path using edge weights."
        )

        path_lengths = [len(path) - 1 for path in learned_paths.values()]
        total_transit_times = []
        for path in learned_paths.values():
            edges = list(zip(path[:-1], path[1:]))
            total_time = sum(weight_dict[u][v] for u, v in edges if u in weight_dict and v in weight_dict[u])
            total_transit_times.append(total_time)


        fig, (ax_text, ax_len, ax_time) = plt.subplots(nrows=3, figsize=(7, 10), gridspec_kw={'height_ratios': [0.5, 1.5, 1.5]})
        fig.subplots_adjust(hspace=0.3) 
        ax_text.axis("off")
        ax_text.text(0, 1, summary, ha='left', va='top', fontsize=8, wrap=True, fontfamily='monospace')
        ax_len.hist(path_lengths, bins=10, alpha=0.7, label="Path Lengths")
        ax_len.set_title("Distribution of Path Lengths")
        ax_len.set_xlabel("Length")
        ax_len.set_ylabel("Frequency")
        ax_len.grid(False)
        ax_time.hist(total_transit_times, bins=10, alpha=0.7, label="Transit Times", color='orange')
        ax_time.set_title("Distribution of Transit Times")
        ax_time.set_xlabel("Time")
        ax_time.set_ylabel("Frequency")
        ax_time.grid(False)

        pdf.savefig(fig)
        plt.close()
        
            
    print(f" PDF report saved to: {output_pdf_path}")







